/*const  ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:40:08 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 02:16:49 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "../include/Irc.hpp"
#include "../channel/Channel.hpp"
#include "../debug/Reflector.hpp"
#include "../data/UserData.hpp"
#include "../data/ChannelData.hpp"
#include "../include/utils.hpp"
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::auto_ptr;
using std::pair;
using IRC::Error;
using IRC::Command;

RequestHandler::RequestHandler() {}
vector<pair<int, vector<string> > >  RequestHandler::get_request(vector<string>& req, const Connection& connection) {

	auto_ptr<Task> task;
	vector<pair<int, vector<string> > > reply;
	try {
		task = Task::create(req, connection);	
	}
	catch(Error &e) {
		std::cerr << e.what() << std::endl;
	}
	try {
		UserTask* user_task = dynamic_cast<UserTask*>(task.get());
		if (user_task != NULL) {
			reply.push_back(make_pair(connection.socket_fd, execute(*user_task)));
		}
		ChannelTask* channel_task = dynamic_cast<ChannelTask*>(task.get());
		if (channel_task != NULL) {
			reply = execute(*channel_task);
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	Reflector::shared().update();
	return reply;
}

vector<pair<int, vector<string> > >
RequestHandler::execute(ChannelTask& task) {
	ChannelData& data = ChannelData::get_storage();
	UserData& Udata = UserData::get_storage();
	User& user = UserData::get_storage().get_user(task.get_connection());
	vector<pair<int, vector<string> > > replies;
	replies.push_back(make_pair(task.get_connection().socket_fd, vector<string>()));
	switch (task.get_command().type) {
		case Command::JOIN:
			if (task.params.size() == 1 && task.params[0] == "0") {
				data.leave_all_joined_channels(user);
				break;
			}
			for (size_t i = 0; i < task.params.size(); ++i) {
				vector<string> name_key = split_string(task.params[i]);
				try {
					const Channel& joined = (name_key.size() == 2 ? 
							data.join_channel(name_key[0], name_key[1], user):
							data.join_channel(name_key[0], user));
					user.add_channel(joined);
					task.add_channel_to_reply(joined);
				}
				catch (Error& e) {
					// TODO: add error no permission
				}
			}
			replies[0].second = task.get_reply();
			break;
		case Command::PART: 
			{
				string reason;
				size_t number_of_channels = task.params.size();
				if (task.params.size() > 1 && 
						task.params.back()[0] != IRC::ChannelLabel::LOCAL_CHANNEL_PREFIX) {
					number_of_channels -= 1;
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
					}
					catch (ChannelData::ChannelNotExist&) {
						task.add_error(Error(Error::ERR_NOSUCHCHANNEL));
					}
					replies[0].second = task.get_reply();
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

vector<string> RequestHandler::execute(const UserTask& task) {
	UserData& data = UserData::get_storage();
	switch (task.get_command().type) {
		case Command::PASS:
			if (!data.is_pedding_user_exist(task.get_connection()))
				data.add_pendding_user(task);
			else 
				data.update_task(task);
			break;
		case Command::NICK:
			if (data.is_duplicated(task.info.nick_name))
				throw Error(Error::ERR_NICKNAMEINUSE);
			else if (data.is_pedding_user_exist(task.get_connection()))
				data.update_task(task);
			else
				data.add_pendding_user(task);
			break;
		case Command::USER: 
			{
				if (!data.is_pedding_user_exist(task.get_connection())) {
					if (UserData::get_storage().is_user_exist(task.get_connection()))
						throw Error(Error::ERR_ALREADYREGISTRED);
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
				return updated.get_reply();
			}
		default:
			throw Command::UnSupported();
	}
	return vector<string>();
}
