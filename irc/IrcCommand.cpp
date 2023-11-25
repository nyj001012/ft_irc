/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:28:24 by heshin            #+#    #+#             */
/*   Updated: 2023/11/18 01:33:05 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcCommand.hpp"
#include "../include/json.hpp"
#include <climits>
#define CMD_STRINGIFY(name) # name

using std::string;
using std::make_pair;

typedef IRC::Command Command;

const char *all_commands[] = {
	CMD_STRINGIFY(PASS),
	CMD_STRINGIFY(USER),
	CMD_STRINGIFY(NICK),
	CMD_STRINGIFY(QUIT),
	CMD_STRINGIFY(JOIN),
	CMD_STRINGIFY(PART),
	CMD_STRINGIFY(MODE),
	CMD_STRINGIFY(TOPIC),
	CMD_STRINGIFY(INVITE),
	CMD_STRINGIFY(PRIVMSG),
	//CMD_STRINGIFY(PING),
	//CMD_STRINGIFY(PONG),
};

Command::Command(): type(Unknown){ }
Command::Command(const string& str) {
	for (size_t i = 0; i < sizeof(all_commands) / sizeof(all_commands[0]); ++i) {
		if (str == all_commands[i]) {
			type = static_cast<Command::Type>(i);
			return ;
		}
	}
	throw Command::UnSupported();
}

const Command::range Command::parameter_range() const {
	Command::range range;
	switch (type) {
		case PASS:
			range = make_pair(1, 1);
			break;
		case NICK:
			range = make_pair(1, 2);
			break;
		case JOIN:
			range = make_pair(1, INT_MAX);
			break;
		default:
			break;
	}
	return range;
}

string Command::_get_label() const {
	if (type == Unknown) {
		throw Command::UnSupported();
	}
	return all_commands[type];
}

const char* Command::UnSupported::what() const throw(){
	return "Unsupported command";
}

std::ostream& Command::_add_to_serialization(std::ostream& os,const int) const {
	_json(os, _get_label());
	return os;
}

bool operator==(const Command& command, const Command::Type& type) {
	return command.type == type;
}

bool operator==(const Command::Type& type, const Command& command) {
	return command.type == type;
}

bool operator!=(const Command& command, const Command::Type& type) {
	return !(command == type);
}

bool operator!=(const Command::Type& type, const Command& command) {
	return !(command == type);
}
#undef CMD_STRINGIFY
