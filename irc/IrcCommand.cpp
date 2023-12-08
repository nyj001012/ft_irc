/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimsejoon <kimsejoon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:28:24 by heshin            #+#    #+#             */
/*   Updated: 2023/12/08 11:49:28 by kimsejoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/json.hpp"
#include "../include/Irc.hpp"
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
	CMD_STRINGIFY(KICK),
	CMD_STRINGIFY(PRIVMSG),
	CMD_STRINGIFY(PING),
	CMD_STRINGIFY(PONG),
};
Command::Command(): type(Unknown){ }
Command::Command(const string& str) {
	for (size_t i = 0; i < sizeof(all_commands) / sizeof(all_commands[0]); ++i) {
		if (str == all_commands[i]) {
			type = static_cast<Command::Type>(i);
			return ;
		}
	}
	throw Error(Error::ERR_UNKNOWNCOMMAND);
}

string Command::get_command_name(const Type t) {
	return all_commands[t];
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
		case PART:
			range = make_pair(1, INT_MAX);
			break;
		case QUIT:
			range = make_pair(0, INT_MAX);
			break;
		case USER:
			range = make_pair(4, 4);
			break;
		case KICK:
		case MODE:
			range = make_pair(2, INT_MAX);
			break;
		case TOPIC:
			range = make_pair(0, 2);
			break;
		case INVITE:
			range = make_pair(2, 2);
			break;
		case PING:
		case PONG:
			range = make_pair(1, 1);
			break;
		default:
			break;
	}
	return range;
}

string Command::_get_label() const {
	if (type == Unknown) {
		throw Error(Error::ERR_UNKNOWNCOMMAND);
	}
	return all_commands[type];
}


std::ostream& Command::_add_to_serialization(std::ostream& os,const int) const {
	_json(os, "type", ':', _get_label());
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
