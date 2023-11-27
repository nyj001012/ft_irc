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

vector<pair<int, vector<string> > >
RequestHandler::execute(ChannelTask& task) {
	ChannelData& data = ChannelData::get_storage();
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
		default:
			throw Command::UnSupported();
	}
	return replies; 
}

vector<pair<int, vector<string> > > RequestHandler::execute(const UserTask& task) {
	UserData& data = UserData::get_storage();
	vector<pair<int, vector<string> > > replies;
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
				replies.push_back(make_pair(task.get_connection().socket_fd, updated.get_reply()));
				break;
			}
		default:
			throw Command::UnSupported();
	}
	return replies;
}
