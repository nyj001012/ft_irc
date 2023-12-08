#include "Task.hpp"
#include "../include/utils.hpp"
#include "../include/json.hpp"
#include "../include/Irc.hpp"
#include <utility>

using std::string;
using std::vector;
using std::auto_ptr;
using IRC::Command;
using IRC::Error;
using std::pair;
using std::make_pair;
typedef std::pair<std::string, const Serializable*> KeyValue;
 
int count_number_of_param(const vector<string>&);
void trim_last_param(vector<string>& params);

Task::Task() {}
Task& Task::operator=(const Task& other) {
	command = other.command;
	return *this;
}

auto_ptr<Task> Task::create(std::vector<std::string>& tokens, const Connection& connection) {
	Task base;
	base.connection = connection;
	if (tokens.front()[0] == ':') {
		base.prefix = tokens.front();
		tokens.erase(tokens.begin());
	}
	base.command = Command(tokens.front());
	tokens.erase(tokens.begin());
	trim_last_param(tokens);
	int count = count_number_of_param(tokens);
	Command::range range = base.command.parameter_range();
	if (count < range.first) {
		if (base.command == Command::PRIVMSG) {
			if (count == 0)
				throw Error(Error::ERR_NORECIPIENT);
			else
				throw Error(Error::ERR_NOTEXTTOSEND);
		}
		else if (base.command == Command::NICK)
			throw Error(Error::ERR_NONICKNAMEGIVEN);
		else
			throw Error(Error::ERR_NEEDMOREPARAMS); 
	}
	else if (count > range.second) {
		// Throw Error?
	}

	switch (base.command.type) {
		case Command::PASS: 
		case Command::NICK: 
		case Command::USER:
		case Command::QUIT:
			return auto_ptr<Task>(new UserTask(base, tokens));
			break;
		case Command::JOIN:
		case Command::PART:
		case Command::TOPIC:
			return auto_ptr<Task>(new ChannelTask(base, tokens));
			break ;
		case Command::MODE:
		case Command::INVITE:
		case Command::KICK:
			return auto_ptr<Task>(new ChannelTask(base, tokens));
		case Command::PRIVMSG:
			return auto_ptr<Task>(new MessageTask(base, tokens));
		case Command::PING:
			return auto_ptr<Task>(new PingTask(base, tokens));
		default:
			throw Error(Error::ERR_UNKNOWNCOMMAND);
			break;
	}
}


void Task::add_error(const Error& error) {
	errors.push_back(error);
}

bool Task::has_error() const {
	return !errors.empty();
}

vector<IRC::Error> Task::get_errors() const {
	return errors;
}

vector<string> Task::get_reply() const {
	return vector<string>();
}

const string& Task::get_prefix() const {
	return prefix;
}

const Command Task::get_command() const {
	return command;
}

const Connection& Task::get_connection() const {
	return connection;
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

void trim_last_param(vector<string>& params) {
	if (params.size() < 1)
		return ;
	int found = -1;
	for (size_t i = 0; i < params.size(); ++i) {
		if (params[i][0] == ':') {
			found = i;
			break;
		}
	}
	if (found == -1)
		return ;
	params[found].erase(0, 1);
	for (size_t i = found + 1; i< params.size(); ++i) {
		params[found] += params[i];	
	}
	params.resize(found + 1);
}

// Serializable
string Task::_get_label() const {
	string label = "Command=" + command._get_label();
	if (!prefix.empty())
		label += " prefix=" + prefix;
	return label;
}

vector<pair<string, const Serializable*> > Task::_get_children() const {
	vector<pair<string, const Serializable*> > vec;
	vec.push_back(make_pair("command", &this->command));
	vec.push_back(make_pair("connection", &this->connection));
	return vec;
}

std::ostream& Task::_add_to_serialization(std::ostream& os, const int) const {
	_json(os, "label", ':', _get_label());
	return os;
}

const char* Task::ParsingFail::what() const throw(){
	return "Fail to parse task";
}

