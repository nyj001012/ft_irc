/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserTask.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 23:48:09 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 02:19:18 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "../include/Irc.hpp"
#include "../include/config.hpp"
#include "../include/utils.hpp"
#include <ctype.h>
#include <cassert>
#include <sstream>
#include <utility>

using std::vector;
using std::string;
using std::make_pair;
using IRC::Command;
using IRC::Reply;
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

UserTask::UserTask(const Task& parent, const vector<string>& raw_params): Task(parent) {
	switch (command.type){
		case Command::PASS:
			connection.password = raw_params.front();
			break;
		case Command::NICK:
			info.nick_name = raw_params.front();
			break;
		case Command::USER:
			info.user_name = raw_params[0];
			info.host_name = raw_params[1];
			info.server_name = raw_params[2];
			info.real_name = raw_params[3];
			break;
		case Command::QUIT:
			if (!raw_params.empty())
				params.push_back(raw_params[0]);
			break;
		default:
			assert(Command::UnSupported().what());
	}
}

UserTask& UserTask::add_next(const UserTask& next) {

	if (command == Command::USER) {
			//throw Error	
		return *this;
	}
	if (next.command == Command::PASS) {
		if (command != Command::PASS) {
			//throw Error	
			return *this;
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

vector<string> UserTask::get_reply() const {
	vector<string> vec;
	if (command != Command::USER)
		return vec;	
	
	vector<string> params = strs_to_vector(WELCOM_MESSAGE + ' '+ info.get_id());
	vec.push_back(Reply(Reply::RPL_WELCOME, SERVER_NAME, params).to_string());
	return vec;
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
	vector<KeyValue> vec = Task::_get_children();
	vec.push_back(make_pair("info", &this->info));
	return vec;
}
