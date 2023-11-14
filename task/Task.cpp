/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 00:25:33 by heshin            #+#    #+#             */
/*   Updated: 2023/11/15 04:01:37 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "../include/utils.hpp"
#include "../include/json.hpp"
#include "../irc/IrcError.hpp"
#include <utility>

using std::string;
using std::vector;
typedef std::pair<std::string, const Serializable*> KeyValue;
 
int count_number_of_param(const vector<string>&);

Task::Task() {}
Task& Task::operator=(const Task& other) {
	command = other.command;
	return *this;
}

Task* Task::create(std::vector<std::string>& tokens) {
	Task base;
	if (tokens.front()[0] == ':') {
		base.prefix = tokens.front();
		tokens.erase(tokens.begin());
	}
	base.command = Command(tokens.front());
	tokens.erase(tokens.begin());
	int count = count_number_of_param(tokens);
	Command::range range = base.command.parameter_range();
	if (count < range.first) {
		throw IrcError(IrcError::ERR_NEEDMOREPARAMS); 
	}
	else if (count > range.second) {
		// TODO: Throw IrcError
	}

	switch (base.command.type) {
		case Command::PASS: 
		case Command::NICK: 
		case Command::USER:
			return new UserTask(base, tokens);
			break;
		default:
			break;
	}
	return NULL;
}


bool Task::has_error() const {
	return true;
}

const string& Task::get_prefix() const {
	return prefix;
}

Task& Task::set_prefix(const string& p) {
	prefix = p;
	return *this;
}

int count_number_of_param(const vector<string>& params) {

	vector<string>::const_iterator iter;
	int count = 0;
	for (iter = params.begin(); iter != params.end(); ++iter) {
		if ((*iter)[0] != ',')
			++count;
	}
	return count;
}

// Serializable
string Task::_get_label() const {
	string label = "Command=" + command._get_label();
	if (!prefix.empty())
		label += " prefix=" + prefix;
	return label;
}

std::ostream& Task::_add_to_serialization(std::ostream& os, const int) const {
	_json(os, "label", ':', _get_label());
	return os;
}

const char* Task::ParsingFail::what() const throw(){
	return "Fail to parse task";
}

