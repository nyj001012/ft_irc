/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserTask.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 23:48:09 by heshin            #+#    #+#             */
/*   Updated: 2023/11/15 01:38:56 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "../irc/IrcCommand.hpp"
#include <cassert>
#include <sstream>
#include <utility>

using std::vector;
using std::string;
using std::make_pair;
typedef std::pair<std::string, const Serializable*> KeyValue;

UserTask::UserTask(const Command command, const vector<string>& params): command(command) {
	switch (command.type){
		case Command::PASS:
			connection.password = params.front();
			break;
		case Command::NICK:
			info.nick_name = params.front();
			break;
		case Command::USER:
			info.user_name = params[0];
			info.host_name = params[1];
			info.server_name = params[2];
			info.real_name = params[3];
			break;
		default:
			assert(command.to_string());
	}
}

UserTask& UserTask::add_next(const UserTask& next) {

	if (command == Command::USER) {
		// TODO: throw IrcError	

	}
	if (next.command == Command::PASS) {
		if (command != Command::PASS) {
			// TODO: throw IrcError	

		}
		connection.password = next.connection.password;
	}
	else if (next.command == Command::NICK) {
		info.nick_name = next.info.nick_name;
	}
	else {
		string nick_name = info.nick_name;
		info.nick_name = nick_name;
		info = next.info;
	}
	command = next.command;
	return *this;
}

bool UserTask::is_ready() const {
	return command == Command::USER;	
}

// Serializable
string UserTask::_get_label() const {
	std::stringstream ss;
	ss << "socket=" << connection.socket_fd;
	if (!info.nick_name.empty()) {
		ss << ", nick name=" << info.nick_name;
	}
	return ss.str();
}

vector<KeyValue> UserTask::_get_children() const {
	vector<KeyValue> vec;
	vec.push_back(make_pair("connection", &this->connection));
	vec.push_back(make_pair("last command", &this->command));
	vec.push_back(make_pair("info", &this->info));
	return vec;
}
