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
#include "../debug/Reflector.hpp"
#include "../data/UserData.hpp"
#include "../data/ChannelData.hpp"
#include "../include/utils.hpp"
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::auto_ptr;
using IRC::Error;
using IRC::Command;

RequestHandler::RequestHandler() {}
vector<string> RequestHandler::get_request(vector<string>& req, const Connection& connection) {

	auto_ptr<Task> task;
	vector<string> reply;
	try {
		task = Task::create(req, connection);	
	}
	catch(Error &e) {
		std::cerr << e.what() << std::endl;
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
	return reply;
}

vector<string> RequestHandler::execute(const ChannelTask& task) {
	ChannelData& data = ChannelData::get_storage();
	User& user = UserData::get_storage().get_user(task.get_connection());
	switch (task.get_command().type) {
		case Command::JOIN:
			if (task.params.size() == 1 && task.params[0] == "0") {
				data.leave_all_joined_channels(user);
				break;
			}
			for (size_t i = 0; i < task.params.size(); ++i) {
				vector<string> name_key = split_string(task.params[i]);
				if (name_key.size() == 2)
					user.add_channel(data.join_channel(name_key[0], name_key[1], user));
				else 
					user.add_channel(data.join_channel(name_key[0], user));
			}
			break;
		default:
			throw Command::UnSupported();
	}
	return vector<string>();
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
