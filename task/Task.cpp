/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 00:25:33 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 19:12:53 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "../include/utils.hpp"

using std::string;
using std::vector;

int count_number_of_param(const vector<string>&);

Task::Task() {}

Task Task::create(std::vector<std::string>& tokens) {
	string prefix;
	if (tokens.front()[0] == ':') {
		prefix = tokens.front();
		tokens.erase(tokens.begin());
	}
	Command command(tokens.front());
	tokens.erase(tokens.begin());
	int count = count_number_of_param(tokens);
	Command::range range = command.parameter_range();
	if (count < range.first || count > range.second) {
		// TODO: Throw IrcError
	}

	Task task;
	switch (command.type) {
		case Command::PASS:
			task = UserTask(command, tokens);
			break;
		default:
			break;
	}
	return task;
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

const char* Task::ParsingFail::what() const throw(){
	return "Fail to parse task";
}

