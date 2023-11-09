/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelData.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:02:30 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 19:14:36 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelData.hpp"
#include "../channel/Channel.hpp"
#include "../user/User.hpp"
#include <utility>

using std::string;
using std::list;
using std::pair;
using std::make_pair;
using std::map;

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
	map<string, list<Channel>::iterator>::const_iterator found = channel_map.find(name);
	if (found == channel_map.end())
		throw ChannelNotExist();
	return *found->second;
}

void ChannelData::join_channel(const string& name, const User& user) {
	map<string, list<Channel>::iterator>::const_iterator found = channel_map.find(name);
	if (found == channel_map.end()) {
		create_channel(name, user);	
	}
	else {
		found->second->add_user(user);
	}
}

void ChannelData::leave_channel(const Channel &channel, const User& user) {
	Channel& channel_ref = const_cast<Channel&>(channel);
	channel_ref.remove_user(user);
	//TODO: Check channel close
}

void ChannelData::create_channel(const string& name, const User& user) {
	channels.push_back(Channel(name, user));
	list<Channel>::iterator new_channel = channels.end();
	--new_channel;
	channel_map.insert(make_pair(name, new_channel));
}

void ChannelData::remove_channel(const Channel& channel) {
	map<string, list<Channel>::iterator>::iterator found = channel_map.find(channel.get_name());
	if (found != channel_map.end()) {
		channel_map.erase(found);
		channels.erase(found->second);
	}
	else {
		channels.remove(channel);
	}
}

const char* ChannelData::ChannelNotExist::what() const throw() {
	return "Channel not exist";
}

const char* ChannelData::ChannelAlreadyExist::what() const throw() {
	return "Channel already exist";
}
