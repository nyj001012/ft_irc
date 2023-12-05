#include "Task.hpp"
#include "../include/utils.hpp"
#include "../include/json.hpp"
#include "../include/Irc.hpp"
#include <sstream>

using IRC::Error;
using std::string;
using std::ostream;
using std::vector;
using std::make_pair;

PingTask::PingTask(const Task& parent, const std::vector<std::string>& raw_params): Task(parent) {
	if (raw_params.size() == 0) {
		add_error(Error(Error::ERR_NOORIGIN));
		return;
	}
	_content = raw_params[0];
}

string PingTask::_get_label() const {
	std::stringstream ss;
	ss << "content=" << _content;
	return ss.str();
}

ostream& PingTask::_add_to_serialization(ostream& os, const int) const {
	_json(os, "content", ": " + _content);
	return os;
}

vector<string> PingTask::get_reply() const {
	vector<string> reply;
	reply.push_back("PONG");
	reply.push_back(_content);
	return reply;
}
