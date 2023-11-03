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
	
Message::Message(const std::string& str): params(vector<string>()) {
	if (str.length() == 0)
		throw Message::ParsingFail();
	vector<string> split = split_string(str);
	if (split.size() == 0)
		throw Message::ParsingFail();
	vector<string>::iterator iter = split.begin();
	if ((*iter)[0] == ':') {
		prefix = iter->substr(1);
		++iter;
		if (iter == split.end())
			throw Message::ParsingFail();
	}
	command = Command(*iter);
	++iter;
	while (iter != split.end()) {
		params.push_back(*iter);
	}
}

const char* Message::ParsingFail::what() {
	return "Fail to parse message";
}

const char* Command::UnSupported::what() {
	return "Unsupported command";
}
#undef CMD_STRINGIFY
