/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:40:08 by heshin            #+#    #+#             */
/*   Updated: 2023/12/08 18:29:34 by sejokim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "../include/Irc.hpp"
#include "../channel/Channel.hpp"
#include "../debug/Reflector.hpp"
#include "../data/UserData.hpp"
#include "../data/ChannelData.hpp"
#include "../include/utils.hpp"
#include "../message/Message.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>

using std::string;
using std::vector;
using std::cout;
using std::auto_ptr;
using std::make_pair;
using std::pair;
using IRC::Error;
using IRC::Command;
using IRC::Reply;

int get_fd(const User*);
Message add_broadcast_to_others(const vector<string>, vector<Message>&, const Channel&, const User&);
Message add_broadcast_to_all(const vector<string>, vector<Message>&, const Channel&);

RequestHandler::RequestHandler() {}


vector<Message>  RequestHandler::get_request(vector<string>& req, const Connection& connection) {

	auto_ptr<Task> task;
	vector<Message> reply;
	try {
		task = Task::create(req, connection);
	}
	catch(Error &e) {
		add_new_message(strs_to_vector(e.get_message()), connection.socket_fd, reply);	
		return reply;
	}
	try {
		UserTask* user_task = dynamic_cast<UserTask*>(task.get());
		if (user_task != NULL) {
			reply = execute(*user_task);
		}
		ChannelTask* channel_task = dynamic_cast<ChannelTask*>(task.get());
		if (channel_task != NULL) {
			reply = execute(*channel_task);
		}
		MessageTask* message_task = dynamic_cast<MessageTask*>(task.get());
		if (message_task != NULL) {
			reply = execute(*message_task);	
		}
		PingTask* ping_task = dynamic_cast<PingTask*>(task.get());
		if (ping_task != NULL) {
			reply = execute(*ping_task);
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	Reflector::shared().update();
	if (task->has_error()) {
		vector<Error> errors = task->get_errors();
		vector<string> error_messages;
		std::transform(errors.begin(), errors.end(), std::back_inserter(error_messages), &Error::_get_message);
		add_new_message(error_messages, connection.socket_fd, reply);
	}
	return reply;
}

vector<Message>
RequestHandler::execute(MessageTask& task) {

	vector<Message> replies = Message::create_start_from(task.get_connection().socket_fd);
	UserData& user_data = UserData::get_storage();
	ChannelData& channel_data = ChannelData::get_storage();

	const User& sender = user_data.get_user(task.get_connection());
	task.set_sender(sender);

	for (size_t i = 0; i < task.recipients.size(); ++i) {
		vector<pair<int, string> > recipients;
		pair<int, string> r;
		const string& name = task.recipients[i].second;

		switch (task.recipients[i].first) {
			case MessageTask::USER:
				if (!user_data.is_user_exist(name))
					task.add_error(Error(Error::ERR_NOSUCHNICK, strs_to_vector(name)));
				else {
					const User& recipient = user_data.get_user(name);
					recipients.push_back(
							make_pair(recipient.get_connection().socket_fd, name));
				}
				break;
			case MessageTask::CHANNEL:
				if (!channel_data.is_channel_exist(name)) 
					task.add_error(Error(Error::ERR_NOSUCHCHANNEL, strs_to_vector(name)));
				else if (!sender.is_joined(name)) 
					task.add_error(Error(Error::ERR_NOTONCHANNEL, strs_to_vector(name)));
				else {
					const Channel& channel = channel_data.get_channel(name);
					vector<const User*> users = channel.get_users();
					for (size_t i = 0; i < users.size(); ++i) {
						if (users[i] != &sender) {
							recipients.push_back(
									make_pair(users[i]->get_connection().socket_fd, name));
						}
					}
				}
				break;
		}

		if (!recipients.empty()) {
			add_new_message(strs_to_vector(task.content), task.get_prefix() + ' ' + Command::get_command_name(Command::PRIVMSG), recipients, replies);
		}
	}

	return replies;
}

vector<Message>
RequestHandler::execute(ChannelTask& task) {
	ChannelData& data = ChannelData::get_storage();
	UserData& user_data = UserData::get_storage();
	User& user = UserData::get_storage().get_user(task.get_connection());
	vector<Message> replies = Message::create_start_from(task.get_connection().socket_fd);
	switch (task.get_command().type) {
		case Command::JOIN:
			if (task.params.size() == 1 && task.params[0] == "0") {
				data.leave_all_joined_channels(user);
				break;
			}
			for (size_t i = 0; i < task.params.size(); ++i) {
				vector<string> name_key = split_string(task.params[i]);
				try {
					// TODO: throw error no permission
					const string& channel_name = task.params[0];
					if (data.is_channel_exist(channel_name)) {
						const Channel& existed_channel = data.get_channel(channel_name);
						if (!existed_channel.can_join()) {
							task.add_error(Error(Error::ERR_CHANNELISFULL, strs_to_vector(user.get_nickname(), channel_name)));
							continue;
						}
						if (existed_channel.is_invite_only() 
								&& !existed_channel.is_invited(user)) {
							task.add_error(Error(Error::ERR_INVITEONLYCHAN, strs_to_vector(user.get_nickname(), channel_name)));
							continue ;
						}
					}
					const Channel* joined = NULL;
					try {
						if (name_key.size() == 2) {
							joined = &data.join_channel(name_key[0], name_key[1], user);
						}
						else {
							joined = &data.join_channel(name_key[0], user);
						}
					} catch(Error& e) {
						task.add_error(e);
						continue;
					}
					user.add_channel(*joined);
					task.add_channel_to_reply(*joined);
					if (joined->get_number_of_users() > 1) {
						string message = ChannelTask::get_channel_join_message(*joined, user);
						add_broadcast_to_others(strs_to_vector(message),
								replies, *joined, user);
					}
					
				}
				catch (Error& e) {
					task.add_error(e);
				}
			}
			add_new_message(task.get_reply(), task.get_connection().socket_fd, replies);
			break;
		case Command::PART: 
			{
				string reason;
				size_t number_of_channels = task.params.size();
				if (task.params.size() > 1 && 
						task.params.back()[0] != IRC::ChannelLabel::LOCAL_CHANNEL_PREFIX) {
					number_of_channels -= 1;
					reason = task.params.back();
				}
				vector<const Channel*> empty_channels;
				for (size_t i = 0; i < number_of_channels; ++i) {
					const string& channel_name = task.params[i];
					if(!user.is_joined(channel_name)) {
						task.add_error(Error(Error::ERR_NOTONCHANNEL, strs_to_vector(channel_name)));
						continue;
					}
					try {
						const Channel& channel = data.get_channel(channel_name);
						data.leave_channel(channel, user);
						user.remove_channel(channel);
						task.add_channel_to_reply(channel);
						if (channel.get_number_of_users() == 0)
							empty_channels.push_back(&channel);
						else {
							string message = ChannelTask::get_channel_part_message(
									channel, user, reason);
							add_broadcast_to_others(strs_to_vector(message),
									replies, channel, user);
						}
					}
					catch (ChannelData::ChannelNotExist&) {
						task.add_error(Error(Error::ERR_NOSUCHCHANNEL, strs_to_vector(channel_name)));
					}
					add_new_message(task.get_reply(), task.get_connection().socket_fd, replies);
					for (size_t i = 0; i < empty_channels.size(); ++i)
						data.remove_channel(*empty_channels[i]);
				}
			}
			break;
		case Command::INVITE:
			{
				if (!user_data.is_user_exist(task.get_connection())) {
					return replies;
				}
				const User& user = user_data.get_user(task.get_connection());
				const string& target_user_name = task.params[0];
				const string& channel_name = task.params[1];
				if (!user_data.is_user_exist(target_user_name)) {
					task.add_error(Error(Error::ERR_NOSUCHNICK, strs_to_vector(target_user_name)));
				}
				else if (!data.is_channel_exist(channel_name))
					task.add_error(Error(Error::ERR_NOSUCHCHANNEL, strs_to_vector(channel_name)));
				else if (!user.is_joined(channel_name)) 
					task.add_error(Error(Error::ERR_NOTONCHANNEL, strs_to_vector(channel_name)));
				else {
					const Channel& channel = data.get_channel(channel_name);
					if (!channel.is_operator(user))
						task.add_error(Error(Error::ERR_CHANOPRIVSNEEDED, strs_to_vector(channel_name)));
					else {
						const User& target_user = user_data.get_user(target_user_name);
						if (target_user.is_joined(channel_name))
							task.add_error(Error(Error::ERR_USERONCHANNEL, strs_to_vector(target_user_name, channel_name)));
						else if (!channel.is_allowed_to_invite(user)) 
							task.add_error(Error(Error::ERR_CHANOPRIVSNEEDED, strs_to_vector(channel_name)));
						else {
							data.invite_user(target_user, channel);	
							add_new_message(task.get_reply(), task.get_connection().socket_fd, replies);
							string message = ":" + user.get_info().get_id() + ' ' + Command::get_command_name(Command::INVITE) + ' ' + target_user_name + ' ' + channel_name;
							add_new_message(strs_to_vector(message), target_user.get_connection().socket_fd, replies);
					}
					}
				}
				break;
			}
		case Command::KICK:
			{
				if (!user_data.is_user_exist(task.get_connection())) 
					return replies;
				const User& user = user_data.get_user(task.get_connection());
				const string& channel_name = task.params[0];
				const string& target_user_name = task.params[1];
				if (!user_data.is_user_exist(target_user_name))
					task.add_error(Error(Error::ERR_NOSUCHNICK));
				else if (!data.is_channel_exist(channel_name))
					task.add_error(Error(Error::ERR_NOSUCHCHANNEL));
				else if (!user.is_joined(channel_name))
					task.add_error(Error(Error::ERR_NOTONCHANNEL));
				else {
					const Channel& channel = data.get_channel(channel_name);
					if (!channel.is_operator(user)) {
						task.add_error(Error(Error::ERR_CHANOPRIVSNEEDED, strs_to_vector(channel_name)));
						break;
					}
					else {
						vector<string> messages;
						string kick_message = ":" + user.get_info().get_id() +
							' ' + Command::get_command_name(Command::KICK) + ' ' + channel_name;
						if (task.params.size() > 2) 
							kick_message += " :" + task.params[2];
						else	
							kick_message += " :" + user.get_nickname();
						messages.push_back(kick_message);
						string part_message = ChannelTask::get_channel_part_message(channel, user_data.get_user(target_user_name), "KICK by " + user.get_nickname());
						messages.push_back(part_message);
						add_new_message(messages, task.get_connection().socket_fd, replies);
						add_broadcast_to_others(messages,
								replies, channel, user);

						data.leave_channel(channel, user_data.get_user(target_user_name));	
					}
				}
				break;
			}
		case Command::TOPIC:
			{
				if (task.params.empty()) {
					task.add_error(Error(Error::ERR_NEEDMOREPARAMS));
					break ;
				}
				const std::string &channel_name = task.params[0];

				try {
					Channel &channel = data.get_channel(channel_name);

					if (task.params.size() == 1) {
						std::string topic = channel.get_topic();
						std::string reply = topic.empty() ? "331 " + user.get_nickname() + " " + channel_name + " :No topic is set"
                                              : "332 " + user.get_nickname() + " " + channel_name + " :" + topic;
            			add_new_message(strs_to_vector(reply), task.get_connection().socket_fd, replies);
					}
					else if (task.params.size() >= 2) { // 채널의 주제를 설정하는 경우에
						if (!channel.is_operator(user) && channel.topic_protected) {
							task.add_error(Error(Error::ERR_CHANOPRIVSNEEDED, strs_to_vector(channel.get_name())));
							break ;
						}
						const std::string &new_topic = task.params[1];
						channel.set_topic(new_topic, user);
						std::string message = "332 " + user.get_nickname() + " " + channel_name + " :" + new_topic;
						add_broadcast_to_all(strs_to_vector(message), replies, channel);
					}
				}
				catch (ChannelData::ChannelNotExist&) {
					task.add_error(Error(Error::ERR_NOSUCHCHANNEL));
				}
				break ;
			}
		case Command::MODE: 
			{
				if (task.params.size() < 2)
				{
					task.add_error(Error(Error::ERR_NEEDMOREPARAMS));
					break;
				}
				const std::string &target = task.params[0];
				const std::string &mode_params = task.params[1];
				std::string broadcast_message = ":" + user.get_info().get_id() + " MODE " + target;

				if (target[0] == IRC::ChannelLabel::LOCAL_CHANNEL_PREFIX)
				{
					try
					{
						Channel &channel = data.get_channel(target);

						if (!channel.is_operator(user))
						{
							task.add_error(Error(Error::ERR_CHANOPRIVSNEEDED, strs_to_vector(channel.get_name())));
							break;
						}

						bool setting = true;
						for (size_t i = 0; i < mode_params.length(); ++i) {
							char modeChar = mode_params[i];

							if (modeChar == '+' || modeChar == '-')
							{
								setting = (modeChar == '+');
								broadcast_message += " ";
								broadcast_message += modeChar; // Append '+' or '-' to broadcast message
								continue;
							}

							switch (modeChar) {
								case 'i':
									channel.set_invite_only(setting);
									broadcast_message += "i";
									break;
								case 't':
									channel.set_topic_protection(setting);
									broadcast_message += "t";
									break;
								case 'k':
									if (setting && (i + 1 < task.params.size()))
									{
										channel.set_key(task.params[i + 1]);
										broadcast_message += "k ";
										broadcast_message += task.params[i + 1];
										i++; // Skip key value in loop
									}
									else
									{
										channel.set_key("");
										broadcast_message += "k";
									}
									break;
								case 'l':
									if (setting && (i + 1 < task.params.size()))
									{
										char *end;
										long limit = std::strtol(task.params[i + 1].c_str(), &end, 10);
										if (*end == '\0' && end != task.params[i + 1].c_str())
										{
											channel.set_user_limit(static_cast<int>(limit));
											broadcast_message += "l ";
											broadcast_message += task.params[i + 1];
											i++; // Skip limit value in loop
										}
										else
											task.add_error(Error(Error::ERR_UNKNOWNMODE));
									} else
									{
										channel.remove_user_limit();
										broadcast_message += "l";
									}
									break;
								case 'o':
									if (i + 1 < task.params.size())
									{
										try
										{
											User &target_user = user_data.get_user(task.params[i + 1]);
											if (setting)
											{
												channel.add_operator(target_user);
												broadcast_message += "o ";
												broadcast_message += task.params[i + 1];
											}
											else
											{
												channel.remove_operator(target_user);
												broadcast_message += "o ";
												broadcast_message += task.params[i + 1];
											}
											i++; // Skip nickname in loop
										}
										catch (UserData::UserNotExist&) {
											task.add_error(Error(Error::ERR_NEEDMOREPARAMS));
										}
									}
									break;
							}
						}
						add_broadcast_to_all(strs_to_vector(broadcast_message), replies, channel);
					} catch (ChannelData::ChannelNotExist&)
					{
						task.add_error(Error(Error::ERR_NOSUCHCHANNEL, strs_to_vector(target)));
					}
				}
				break;
			}
		default:
			throw Error(Error::ERR_UNKNOWNCOMMAND);
	}
	return replies;
}

vector<Message> RequestHandler::execute(UserTask& task) {
	UserData& data = UserData::get_storage();
	vector<Message> replies = Message::create_start_from(task.get_connection().socket_fd);
	switch (task.get_command().type) {
		case Command::PASS:
			if (!data.is_pedding_user_exist(task.get_connection()))
				data.add_pendding_user(task);
			else 
				data.update_task(task);
			break;
		case Command::NICK:
			if (data.is_duplicated(task.info.nick_name)) {
				task.add_error(Error(Error::ERR_NICKNAMEINUSE, strs_to_vector(task.info.nick_name)));
				return replies;
			}
			else if (data.is_pedding_user_exist(task.get_connection()))
				data.update_task(task);
			else
				data.add_pendding_user(task);
			break;
		case Command::USER: 
			{
				if (!data.is_pedding_user_exist(task.get_connection())) {
					if (UserData::get_storage().is_user_exist(task.get_connection()))  {
						task.add_error(Error(Error::ERR_ALREADYREGISTRED));
						return replies;
					}
					else {
						// throw error?
						break;
					}
				}
				const UserTask updated = data.update_task(task);
				if (updated.info.nick_name.empty()) {
					// throw error?
					break;
				}
				data.create_user(updated.get_connection(),updated.info);
				data.remove_task(updated.get_connection());
				add_new_message(updated.get_reply(), task.get_connection().socket_fd, replies);
				break;
			}
		case Command::QUIT: 
			{
				if (!data.is_user_exist(task.get_connection()))
					return replies;
				User& user = data.get_user(task.get_connection());
				if (!task.params.empty()) {
					vector<const Channel*> channels = user.get_all_channels();
					string prefix = user.get_info().get_id() + ' ' + Command::get_command_name(Command::QUIT);
					vector<string> messages = strs_to_vector(prefix + " :" + task.params[0]);
					for (size_t i = 0; i < channels.size(); ++i) {
						add_broadcast_to_others(messages, replies, *channels[i], user);
					}
				}
				data.delete_user(user);
				break;
			}
		default:
			throw Error(Error::ERR_UNKNOWNCOMMAND);
	}
	return replies;
}

vector<Message> RequestHandler::execute(PingTask& task) {
	vector<Message> replies = Message::create_start_from(task.get_connection().socket_fd);
	UserData& data = UserData::get_storage();
	if (data.is_user_exist(task.get_connection())) {
		const User& user = data.get_user(task.get_connection());
		vector<string> reply = task.get_reply();
		reply[0] += " :" + user.get_nickname();
		add_new_message(reply, task.get_connection().socket_fd, replies);
	}
	return replies;
}

Message add_broadcast_to_others(const vector<string> new_messages, vector<Message>& messages, const Channel& channel, const User& sender) {

	vector<const User*> users = channel.get_users();	
	vector<int> fds;

	for (size_t i = 0; i < users.size(); ++i) {
		if (users[i]->get_nickname() == sender.get_nickname())
			continue;
		fds.push_back(get_fd(users[i]));
	}
	return add_new_message(new_messages, fds, messages);
}

Message add_broadcast_to_all(const vector<string> new_messages, vector<Message>& messages, const Channel& channel) {

	vector<const User*> users = channel.get_users();	
	vector<int> fds;

	for (size_t i = 0; i < users.size(); ++i) {
		fds.push_back(get_fd(users[i]));
	}
	return add_new_message(new_messages, fds, messages);
}

int get_fd(const User* user) {
	return user->get_connection().socket_fd;
}
