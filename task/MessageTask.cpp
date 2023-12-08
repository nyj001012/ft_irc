/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageTask.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 01:59:01 by heshin            #+#    #+#             */
/*   Updated: 2023/11/30 03:32:41 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "../include/utils.hpp"
#include "../include/json.hpp"
#include "../include/Irc.hpp"
#include "../include/utils.hpp"
#include <sstream>

using IRC::Error;
using std::string;
using std::ostream;
using std::vector;
using std::make_pair;

MessageTask::MessageTask(const Task& parent, const std::vector<std::string>& raw_params): Task(parent), content(raw_params[1]), sender(NULL) {
	vector<string> targets = split_string(raw_params[0], ',');

	for (size_t i = 0; i < targets.size(); ++i) {
		const string& target = targets[i];
		RecipientType type;
		if (target.length() == 0) {
			add_error(Error(Error::ERR_NORECIPIENT));
			continue;
		}
		if (target[0] == IRC::ChannelLabel::LOCAL_CHANNEL_PREFIX) {
			if (!ChannelTask::is_valid_channel_name(target)) {
				add_error(Error(Error::ERR_CANNOTSENDTOCHAN, strs_to_vector(target)));
				continue;
			}
			type = CHANNEL;
		}
		else {
			if (!UserTask::is_valid_nickname(target)) {
				add_error(Error(Error::ERR_NOSUCHNICK, strs_to_vector(target)));
				continue;
			}
			type = USER;
		}
		recipients.push_back(make_pair(type, target));
	}
}

void MessageTask::set_sender(const User& user) {
	if (prefix.empty() || (sender != NULL && sender != &user)) {
		prefix = user.get_info().get_id();	
	}
	sender = &user;
}

typedef std::pair<std::string, const Serializable*> KeyValue;

// Serialize
string MessageTask::_get_label() const {
	std::stringstream ss;
	ss << "number of recipients=" << recipients.size() << ','
		<< "content=" << content;
	return ss.str();
}

template <> 
std::ostream& _json(std::ostream& os, MessageTask::Recipient recipient) {
	switch (recipient.first) {
		case MessageTask::USER:
			os << "USER";
			break;
		case MessageTask::CHANNEL:
			os << "CHANNEL";
			break;
	}
	os << ' ' << recipient.second;
	return os;
}

ostream& MessageTask::_add_to_serialization(ostream& os, const int depth) const {
	if (depth <= 0) {
		_json(os, "label", ':', _get_label());
		return os;
	}
	_json(os, "content", ':', content);
	_json(os, "number of recipents" , ':', recipients.size(), ',');
	if (recipients.size() > 0) {
		os << ',';
		_json(os, "recipients", ':');
		_json(os, recipients);
	}
	return os;
}

vector<KeyValue> MessageTask::_get_children() const {
	vector<KeyValue> vec = Task::_get_children();
	vec.push_back(make_pair("sender", sender));
	return vec;
}
