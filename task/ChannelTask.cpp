#include "Task.hpp"
#include "../channel/Channel.hpp"
#include "../data/UserData.hpp"
#include "../include/json.hpp"
#include "../include/utils.hpp"
#include "../include/config.hpp"
#include <sstream>
#include <utility>

using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::ostream;
using IRC::ChannelLabel;
using IRC::Reply;
using IRC::Error;

void add_channel_join_reply(const Channel&, const User&, vector<string>&);

ChannelTask::ChannelTask(const Task& parent, const vector<string>& raw_params): Task(parent) {
	switch (command.type) {
		case Command::JOIN:
		case Command::PART: 
			if (command == Command::JOIN && 
					params.size() == 1 && params[0] == "0")
				return ;
			params = split_string(raw_params[0], ',');
			for (size_t i = 0; i < params.size(); ++i) {
				if (!is_valid_channel_name(params[i]))
					throw Error(Error::ERR_NOSUCHCHANNEL);
			}
			if (command == Command::JOIN && raw_params.size() > 1) {
				vector<string> keys = split_string(raw_params[1], ',');
				if (keys.size() > params.size()) 
					keys.resize(params.size()); //TODO: error
				for (size_t i = 0; i < keys.size(); ++i) {
					params[i] += ' ' + keys[i];
				}
			}
			else if (command == Command::PART && raw_params.size() > 2) {
				params.push_back(raw_params[1]);
			}
			break;
		case Command::MODE:
			{
			const string& channel_name = raw_params[0];
			if (isalpha(channel_name[0])) {
				throw Error(Error::ERR_UNKNOWNCOMMAND);
			}
			if (!is_valid_channel_name(channel_name)) {
				throw Error(Error::ERR_NOSUCHCHANNEL);
			}
			for (size_t i = 0; i < raw_params.size(); ++i) {
				params.push_back(raw_params[i]);
			}
			break ;
		}
		case Command::TOPIC:
		{
		    const string& channel_name = raw_params[0];
			if (!is_valid_channel_name(channel_name)) {
				throw Error(Error::ERR_NOSUCHCHANNEL);
			}
			params.push_back(channel_name);
			if (raw_params.size() > 1) {
				params.push_back(raw_params[1]);
			}
			break ;
		}
		case Command::INVITE: 
		case Command::KICK:
			{
				const string& user_name = command == Command::INVITE ? raw_params[0]: raw_params[1];
				const string& channel_name = command == Command::INVITE ? raw_params[1]: raw_params[0];
				if (!UserTask::is_valid_nickname(user_name)) {
					throw Error(Error::ERR_NOSUCHNICK);
				}
				if (!is_valid_channel_name(channel_name)) {
					// not on rfc
					throw Error(Error::ERR_NOSUCHCHANNEL);
				}
				params = raw_params;
				break;
			}
		default:
			throw Error(Error::ERR_UNKNOWNCOMMAND);
	}
}

void ChannelTask::add_channel_to_reply(const Channel& channel) {
	channels_to_reply.push_back(&channel);
}

bool ChannelTask::is_valid_channel_name(const string& name) {
	if (name.length() <= 1 || name.length() > 200)
		return false;
	if (name[0] != '&')
		return false;
	for (int i = 0; ChannelLabel::NOT_ALLOWED_CHARS[i] != '\0'; ++i) {
		if (name.find(ChannelLabel::NOT_ALLOWED_CHARS[i]) != string::npos)
			return false;
	}
	return true;
}

vector<string> ChannelTask::get_reply() const {
	vector<string> vec;
	const User& user = UserData::get_storage().get_user(connection);

	if (command == Command::JOIN && !params.empty() && params[0] != "0") {
		vector<const Channel*>::const_iterator iter;
		for (iter = channels_to_reply.begin(); iter != channels_to_reply.end(); ++iter) {
			add_channel_join_reply(**iter, user, vec);
		}
	}
	else if (command == Command::PART) {
		string reason;
		size_t number_of_channels = params.size();
		if (params.size() > 1 && 
				params.back()[0] != IRC::ChannelLabel::LOCAL_CHANNEL_PREFIX) {
			reason = params.back();
			number_of_channels -= 1;
		}
		for (size_t i = 0; i < number_of_channels; ++i) {
			const Channel& channel = *channels_to_reply[i];
			vec.push_back(get_channel_part_message(channel, user, reason));
		}
	}
	else if (command == Command::INVITE) {
		vec.push_back(Reply(Reply::RPL_INVITING, string()).to_string() + ' ' + params[0] + ' ' + params[1]);	
	}
	return vec;
}

string ChannelTask::get_channel_part_message(const Channel& channel, const User& user, const string& reason) {

	const string user_id = ":" + user.get_info().get_id();
	string reply = user_id + " PART " + channel.get_name(); 
	if (!reason.empty())
		reply += " :" + reason;
	return reply;
}

string ChannelTask::get_channel_join_message(const Channel &channel, const User &user) {
	return ":" + user.get_info().get_id() + " JOIN " + channel.get_name();
}

void add_channel_join_reply(const Channel& channel, const User& user, vector<string>& vec){
	
	vec.push_back(ChannelTask::get_channel_join_message(channel, user));
	string mode; //TODO: Channel mode
	vec.push_back(":" + SERVER_NAME + " MODE " + channel.get_name() + ' ' + mode);	
	
	string topic = channel.get_topic();
	if (topic.empty())
		topic = ":";
	vec.push_back(Reply(Reply::RPL_TOPIC, SERVER_NAME, 
				strs_to_vector(user.get_nickname(), channel.get_name(), topic)).to_string());
	vector<string> name_params = strs_to_vector(user.get_nickname(), channel.get_name());
	vector<const User*> users = channel.get_users();
	for (size_t i = 0; i < users.size(); ++i) {
		if (channel.is_operator(*users[i])) 
			name_params.push_back(string("@") + users[i]->get_nickname());
		else 
			name_params.push_back((users[i]->get_nickname()));
	}
	vec.push_back(Reply(Reply::RPL_NAMREPLY, SERVER_NAME, name_params).to_string());
	vec.push_back(Reply(Reply::RPL_ENDOFNAMES, SERVER_NAME, 
				strs_to_vector(user.get_nickname(), channel.get_name())).to_string());
}

// Serialize
string ChannelTask::_get_label() const {
	std::stringstream ss;
	ss << "command=" << command._get_label() << ','
		<< "params=" << params.size();
	return ss.str();
}

ostream& ChannelTask::_add_to_serialization(ostream& os, const int depth) const {
	if (depth <= 0) {
		_json(os, "label", ':', _get_label());
		return os;
	}
	_json(os, "number of params", ':',params.size());
	if (params.size() > 0) {
		os << ',';
		_json(os, "params", ':');
		_json(os, params);
	}
	return os;
}
