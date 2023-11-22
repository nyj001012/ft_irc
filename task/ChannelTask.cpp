/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelTask.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 00:26:59 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 02:04:07 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "../channel/Channel.hpp"
#include "../include/json.hpp"
#include "../include/utils.hpp"
#include <sstream>
#include <utility>

using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::ostream;

bool is_valid_channel_name(const string&);

ChannelTask::ChannelTask(const Task& parent, const vector<string>& raw_params): Task(parent) {
	params = split_string(raw_params[0], ',');
	if (raw_params.size() > 1) {
		vector<string> keys = split_string(raw_params[1], ',');
		if (keys.size() > params.size()) {
			//TODO: error
			keys.resize(params.size());
		}
		for (size_t i = 0; i < keys.size(); ++i) {
			params[i] += ' ' + keys[i];
		}
	}
}

bool is_valid_channel_name(const string&) {
	return true;
}

vector<string> ChannelTask::get_reply() const {
	return vector<string>();
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
		_json(os, "params", ':');
		_json(os, params);
	}
	return os;
}
