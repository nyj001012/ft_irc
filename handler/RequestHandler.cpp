/*const  ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:40:08 by heshin            #+#    #+#             */
/*   Updated: 2023/11/30 03:21:10 by heshin           ###   ########.fr       */
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
using std::pair;
using IRC::Error;
using IRC::Command;

int get_fd(const User*);
void add_broadcast_to_others(const vector<string>, vector<Message>&, const Channel&, const User&);

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
		vector<int> fds;
		const string& name = task.recipients[i].second;

		switch (task.recipients[i].first) {
			case MessageTask::USER:
				if (!user_data.is_user_exist(name))
					task.add_error(Error(Error::ERR_NOSUCHNICK));
				else {
					const User& recipient = user_data.get_user(name);
					fds.push_back(recipient.get_connection().socket_fd);	
				}
				break;
			case MessageTask::CHANNEL:
				if (!channel_data.is_channel_exist(name)) 
					task.add_error(Error(Error::ERR_NOSUCHCHANNEL));
				else if (!sender.is_joined(name)) 
					task.add_error(Error(Error::ERR_NOTONCHANNEL));
				else {
					const Channel& channel = channel_data.get_channel(name);
					vector<const User*> users = channel.get_users();
					std::transform(users.begin(), users.end(), std::back_inserter(fds), get_fd);
				}
				break;
		}

		if (!fds.empty()) {
			add_new_message(
					strs_to_vector(":" + task.get_prefix() + ' ' + task.content),
					fds, replies);
		}
	}

	return replies;
}

vector<Message>
RequestHandler::execute(ChannelTask& task) {
	ChannelData& data = ChannelData::get_storage();
	UserData& Udata = UserData::get_storage();
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
					const Channel& joined = (name_key.size() == 2 ? 
							data.join_channel(name_key[0], name_key[1], user):
							data.join_channel(name_key[0], user));
					user.add_channel(joined);
					task.add_channel_to_reply(joined);
					if (joined.get_number_of_users() > 1) {
						string message = ChannelTask::get_channel_join_message(joined, user);
						add_broadcast_to_others(strs_to_vector(message),
								replies, joined, user);
					}
				}
				catch (Error& e) {
					task.add_error(e);
				}
			}
			add_new_message(task.get_reply(), task.get_connection().socket_fd,replies);
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
						task.add_error(Error(Error::ERR_NOTONCHANNEL));
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
						task.add_error(Error(Error::ERR_NOSUCHCHANNEL));
					}
					add_new_message(task.get_reply(), task.get_connection().socket_fd, replies);
					for (size_t i = 0; i < empty_channels.size(); ++i)
						data.remove_channel(*empty_channels[i]);
				}
			}
			break;
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
					// if (!topic.empty()) {
						// "332 " + user.get_nickname() + " " + channel_name + " :" + topic
					// }
					// else
					// {
						// "331 " + user.get_nickname() + " " + channel_name + " :No topic is set"
					// }
				}
				else if (task.params.size() >= 2) { // 채널의 주제를 설정하는 경우에
					if (!channel.is_operator(user) && channel.topic_protected) {
						task.add_error(Error(Error::ERR_CHANOPRIVSNEEDED));
						break ;
					}
					const std::string &new_topic = task.params[1];
					channel.set_topic(new_topic, user);
					// 332 + user.get_nickname() + " " + channel_name + " :" + new_topic
				}
			}
			catch (ChannelData::ChannelNotExist&) {
				task.add_error(Error(Error::ERR_NOSUCHCHANNEL));
			}
			break ;
		}
		case Command::MODE:
		{
			if (task.params.size() < 2) {
				task.add_error(Error(Error::ERR_NEEDMOREPARAMS));
				break ;
			}

			const std::string &target = task.params[0];
			const std::string &mode = task.params[1];

			if (target[0] == IRC::ChannelLabel::LOCAL_CHANNEL_PREFIX) {
				try {
					Channel &channel = data.get_channel(target);

					if (!channel.is_operator(user))
					{
						task.add_error(Error(Error::ERR_CHANOPRIVSNEEDED));
						break ;
					}
					bool setting = true;
					for (size_t i = 0 ; i < mode.length() ; ++i) {
						char modeChar = mode[i];

						if (modeChar == '+' || modeChar == '-') {
							setting = (modeChar == '+');
							continue ;
						}

						switch (modeChar) {
							case 'i': {
								channel.set_invite_only(setting);
								break;
							}
							case 't':
							{
								channel.set_topic_protection(setting);
								break;
							}
							case 'k':
							{
								if (setting && (i + 1 < task.params.size())) {
									channel.set_key(task.params[i + 1]);
									i++;
								} else {
									channel.set_key("");
								}
								break ;
							}
							case 'l':
							{
								if (setting && (i + 1 < task.params.size())) {
									char *end;
									long limit = std::strtol(task.params[i + 1].c_str(), &end, 10);

									if (*end == '\0' && end != task.params[i + 1].c_str())
										channel.set_user_limit(static_cast<int>(limit));
									else
										task.add_error(Error(Error::ERR_UNKNOWNMODE));
									i++;
								}
								else
									channel.remove_user_limit();
								break ;
							}
							case 'o':
							{
								if (i + 1 < task.params.size()) {
									try {
										User &target_user = Udata.get_user(task.params[i + 1]);
										if (setting)
											channel.add_operator(target_user);
										else
											channel.remove_operator(target_user);
										i++;
									}
									catch (UserData::UserNotExist&) {
										task.add_error(Error(Error::ERR_NEEDMOREPARAMS));
									}
									break ;
								}
							}
						}
					}
				}
				catch (ChannelData::ChannelNotExist&) {
					task.add_error(Error(Error::ERR_NOSUCHCHANNEL));
				}
			}
		}
		default:
			throw Command::UnSupported();
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
				task.add_error(Error(Error::ERR_NICKNAMEINUSE));
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
		default:
			throw Command::UnSupported();
	}
	return replies;
}

void add_broadcast_to_others(const vector<string> new_messages, vector<Message>& messages, const Channel& channel, const User& sender) {

		vector<int> fds;
		vector<const User*> users = channel.get_users();
		std::transform(users.begin(), users.end(), std::back_inserter(fds), get_fd);
		add_new_message(new_messages, fds, messages)
			.remove_fd(sender.get_connection().socket_fd);
}

int get_fd(const User* user) {
	return user->get_connection().socket_fd;
}
