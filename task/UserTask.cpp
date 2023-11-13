/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserTask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 23:48:09 by heshin            #+#    #+#             */
/*   Updated: 2023/11/13 23:48:09 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "../irc/IrcCommand.hpp"
#include <cassert>

using std::vector;
using std::string;

UserTask::UserTask(const Command command, const vector<string>& params): command(command) {
	switch (command.type){
		case Command::PASS:
			password = params.front();
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
		password = next.password;
	}
	else if (next.command == Command::NICK) {
		info.nick_name = next.info.nick_name;
	}
	else {
		info = next.info;
	}
	command = next.command;
	return *this;
}
