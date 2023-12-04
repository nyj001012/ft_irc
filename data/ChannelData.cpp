/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelData.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:02:30 by heshin            #+#    #+#             */
/*   Updated: 2023/12/04 17:50:49 by sejokim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelData.hpp"
#include "../channel/Channel.hpp"
#include "../user/User.hpp"
#include "../include/json.hpp"
#include <utility>
#include <sstream>

using std::string;
using std::list;
using std::make_pair;
using std::map;
using std::stringstream;
using std::vector;
using std::ostream;
using IRC::Error;

ChannelData::ChannelData() {}
ChannelData::~ChannelData() {
	//TODO: Disconnect
}

ChannelData& ChannelData::get_storage() {
	static ChannelData storage;

	return storage;
}

bool ChannelData::is_channel_exist(const string& name) const {
	return channel_map.find(name) != channel_map.end(); 
}

Channel& ChannelData::get_channel(const string& name) const {
	map<string, const Channel*>::const_iterator found = channel_map.find(name);
	if (found == channel_map.end())
		throw ChannelNotExist();
	return *const_cast<Channel *>(found->second);
}

Channel& ChannelData::join_channel(const string& name, const string& key, const User& user) {

	map<string, const Channel*>::const_iterator found = channel_map.find(name);
	if (found == channel_map.end()) {
		Channel& new_channel = create_channel(name, user);	
		if (!key.empty())
			new_channel.set_key(key, user);
		return new_channel;
	}
	else {
		Channel& channel= *const_cast<Channel *>(found->second);
		if (channel.get_key() != key)
			throw Error(Error::ERR_BADCHANNELKEY);
		channel.add_user(user);
		return channel;
	}
}

Channel& ChannelData::join_channel(const string& name, const User& user) {
	return join_channel(name, string(), user);
}

void ChannelData::leave_all_joined_channels(const User& user) {
	vector<const Channel*> channels = user.get_all_channels();
	for (size_t i = 0; i < channels.size(); ++i) {
		leave_channel(*channels[i], user);
	}
}

void ChannelData::leave_channel(const Channel &channel, const User& user) {
	Channel& channel_ref = const_cast<Channel&>(channel);
	channel_ref.remove_user(user);
}

Channel& ChannelData::create_channel(const string& name, const User& user) {
	Channel ch = Channel(name, user);
	channels.push_back(ch);
	Channel& new_channel = channels.back();
	channel_map.insert(make_pair(name, &new_channel));
	return new_channel;
}

void ChannelData::remove_channel(const Channel& channel) {
	map<string, const Channel*>::iterator found = channel_map.find(channel.get_name());
	if (found != channel_map.end()) {
		channel_map.erase(found);
	}
	channels.remove(channel);
}

// Serializable

string ChannelData::_get_label() const {
	stringstream ss;
	ss << "Channel data(number of channel=" << channels.size() << ')';
	return ss.str();
}

ostream& ChannelData::_add_to_serialization(ostream& os, const int depth) const {

	_json(os, "number of channels", ':', channels.size());
	if (depth > 0 && channels.size() > 0) {
		os << ',';
		vector<const Serializable*> vec;
		list<Channel>::const_iterator iter;
		for (iter = channels.begin(); iter != channels.end(); ++iter) {
			vec.push_back(&*iter);
		}
		_json(os, "channels", ':');
		os << ::_serialize(vec, depth - 1);
		if (channel_map.size() > 0) {
			os << ',';
			map<string, const Serializable*> m;
			map<string, const Channel*>::const_iterator iter;
			for (iter = channel_map.begin(); iter != channel_map.end(); ++iter) {
				m.insert(make_pair(iter->first, &*iter->second));
			}
			_json(os, "channel by name", ':');
			os << ::_serialize(m, depth - 1);
		}
	}
	return os;
}

const char* ChannelData::ChannelNotExist::what() const throw() {
	return "Channel not exist";
}

const char* ChannelData::ChannelAlreadyExist::what() const throw() {
	return "Channel already exist";
}
