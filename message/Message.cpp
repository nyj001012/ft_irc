/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 00:25:33 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 00:25:33 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "../include/utils.hpp"
# define CMD_STRINGIFY(name) # name

using std::string;
using std::vector;

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
	//PING,
	//PONG
};

Command::Command() {}

Command::Command(const string& str) {
	for (size_t i = 0; i < sizeof(all_commands) / sizeof(all_commands[0]); ++i) {
		if (str == all_commands[i]) {
			type = static_cast<Command::Type>(i);
			return ;
		}
	}
	throw Command::UnSupported();
}

string Command::type_string() const {
	return all_commands[type];
}

const char* Message::ParsingFail::what() {
	return "Fail to parse message";
}

const char* Command::UnSupported::what() {
	return "Unsupported command";
}
#undef CMD_STRINGIFY
