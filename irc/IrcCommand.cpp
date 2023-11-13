/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:28:24 by heshin            #+#    #+#             */
/*   Updated: 2023/11/13 04:28:24 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcCommand.hpp"
#define CMD_STRINGIFY(name) # name

using std::string;
using std::make_pair;

const char *all_commands[] = {
	CMD_STRINGIFY(Command::NICK),
	CMD_STRINGIFY(Command::INVITE),
	CMD_STRINGIFY(Command::TOPIC),
	CMD_STRINGIFY(Command::MODE),
	CMD_STRINGIFY(Command::PASS),
	CMD_STRINGIFY(Command::USER),
	CMD_STRINGIFY(Command::NICK),
	CMD_STRINGIFY(Command::QUIT),
	CMD_STRINGIFY(Command::JOIN),
	CMD_STRINGIFY(Command::PART),
	CMD_STRINGIFY(Command::MODE),
	CMD_STRINGIFY(Command::TOPIC),
	CMD_STRINGIFY(Command::INVITE),
	CMD_STRINGIFY(Command::PRIVMSG),
	//CMD_STRINGIFY(Command::PING),
	//CMD_STRINGIFY(Command::PONG),
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

const char* Command::to_string() const {
	if (type == Unknown) {
		throw Command::UnSupported();
	}
	return all_commands[type];
}

const Command::range Command::parameter_range() const {
	Command::range range;
	switch (type) {
		case PASS:
			range = make_pair(1, 1);
			break;
		case NICK:
			range = make_pair(1, 2);
		default:
			break;
	}
	return range;
}

const char* Command::UnSupported::what() const throw(){
	return "Unsupported command";
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
