/*const  ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:40:08 by heshin            #+#    #+#             */
/*   Updated: 2023/11/29 23:59:38 by heshin           ###   ########.fr       */
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
RequestHandler::execute(ChannelTask& task) {
	ChannelData& data = ChannelData::get_storage();
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
