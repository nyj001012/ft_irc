/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserTask.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 23:48:09 by heshin            #+#    #+#             */
/*   Updated: 2023/11/18 02:30:39 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "../include/Irc.hpp"
#include <ctype.h>
#include <cassert>
#include <sstream>
#include <utility>

using std::vector;
using std::string;
using std::make_pair;
using IRC::Command;
typedef std::pair<std::string, const Serializable*> KeyValue;

bool UserTask::is_valid_nickname(const string& nick) {

	if (nick.length() < 1 || nick.length() > 9)
		return false;
	const char c = nick[0];
	if (!isalpha(c) && !IRC::is_special(c))
		return false;
	for (size_t i = 1; i < nick.length(); ++i) {
		if(c != '-' && !IRC::is_satisfy_any<
				IRC::is_letter, IRC::is_special, IRC::is_digit>(c))
			return false;
	}
	return true;
}

UserTask::UserTask(const Task& parent, const vector<string>& params): Task(parent) {
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
		if (!is_valid_nickname(next.info.nick_name))
			throw IRC::Error(IRC::Error::ERR_ERRONEUSNICKNAME);
		info.nick_name = next.info.nick_name;
	}
	else {
		string nick_name = info.nick_name;
		info = next.info;
		info.nick_name = nick_name;
	}
	command = next.command;
	return *this;
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
