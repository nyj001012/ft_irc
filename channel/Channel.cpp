/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:42:07 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 01:31:39 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "../user/User.hpp"
#include "../include/json.hpp"
#include "../debug/Serializable.hpp"
#include "../include/Irc.hpp"
#include <algorithm>
#include <ostream>
#include <iterator>
#include <sstream>

using std::string;
using std::vector;
using std::pair;
using std::ostream;
using std::stringstream;
using std::make_pair;
using IRC::Error;

typedef std::pair<string, const Serializable*> KeyValue;

Channel::Channel(): operator_user(NULL) {}
Channel::~Channel() { }
Channel::Channel(const string& name, const User& operator_user)
	:name(name),
	key(string()),
	users(vector<const User*>()),
	operator_user(&operator_user)
{ 
	users.push_back(&operator_user);
}

Channel::Channel(const Channel& other)
	:name(other.name), 
	topic(other.topic),
	users(other.users),
	operator_user(other.operator_user)
{ }

void Channel::set_key(const string& new_key, const User& user) {
	if (&user != operator_user) 
		throw Error(Error::ERR_CHANOPRIVSNEEDED);
	key = new_key;
}

const string& Channel::get_key() const {
	return key;
}

const string& Channel::get_name() const {
	return name;
}

const string& Channel::get_topic() const {
	return topic;
}

vector<const User*> Channel::get_users() const {
	return users;
}

int Channel::get_number_of_users() const {
	return users.size();
}

const User& Channel::get_operator() const {
	return *operator_user;
}

vector<string> Channel::get_user_names() const {
	vector<string> vec;

	vector<const User*>::const_iterator iter;
	for (iter = users.begin(); iter != users.end(); ++iter) {
		vec.push_back((*iter)->get_nickname());
	}
	return vec;
}

void Channel::add_user(const User& user) {
	users.push_back(&user);
}

void Channel::remove_user(const User& user) {
	vector<const User*>::iterator found = std::find(users.begin(), users.end(), &user);
	if (found == users.end())
		return ;
	users.erase(found);
	//TODO: Check operator
}

bool Channel::is_equal(const Channel& other) const {
	if (name != other.name || topic != other.topic)
		return false;
	if (users != other.users)
		return false;

	return true;
}

// Serializable

string Channel::_get_label() const {
	return "name:" + name + ", topic:" + topic;
}

vector<KeyValue> Channel::_get_children() const {
	vector<KeyValue> v;
	v.push_back(make_pair("operator", operator_user));
	return v;
}

ostream& Channel::_add_to_serialization(ostream& os, const int depth) const {
	_json(os, "name", ':', name, ',');
	_json(os, "topic", ':', topic, ',');
	if (depth > 0) {
		const vector<const Serializable *> vec(users.begin(), users.end());
		_json(os, "users", ':') << ::_serialize(vec, depth - 1);
	}
	else {
		_json(os, "users", ':', users.size());
	}
	return os;
}

bool operator==(const Channel& a, const Channel& b) {
	return a.is_equal(b);
}

Channel& Channel::operator=(const Channel& other) {
	(void)other;
	return *this;
}

const char* Channel::AlreadyJoined::what() const throw() {
	return "Already joined to channel";
}

const char* Channel::NoPermission::what() const throw() {
	return "No permission";
}
