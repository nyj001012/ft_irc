/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:42:07 by heshin            #+#    #+#             */
/*   Updated: 2023/12/05 16:41:50 by sejokim          ###   ########.fr       */
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

const string& Channel::get_key() const {
	return key;
}

const string& Channel::get_name() const {
	return name;
}

void	Channel::set_topic(const std::string &new_topic, const User &user)
{
	if (topic_protected && !is_operator(user))
		throw Error(Error::ERR_CHANOPRIVSNEEDED);
	topic = new_topic;
	// mode -t 가 설정이 되어있으면 operator가 아니면 topic을 바꿀 수 없다.
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
	if (&user == operator_user && !users.empty()) 
		operator_user = users[0];	
}

bool Channel::is_equal(const Channel& other) const {
	if (name != other.name || topic != other.topic)
		return false;
	if (users != other.users)
		return false;

	return true;
}

bool	Channel::is_operator(const User &user) const
{
	return &get_operator() == &user;
}

bool Channel::is_invite_only() const
{
	return invite_only;
}

bool Channel::is_allowed_to_join(const User& user) const {
	if (invite_only) {
		return false;
	}
	(void) user;
	return true;
}

bool Channel::is_allowed_to_invite(const User& user) const {
	if (!invite_only)
		return true;
	return is_operator(user);
}

bool Channel::can_join() const {
	return user_limit == -1 || users.size() < static_cast<size_t>(user_limit);
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

void	Channel::set_invite_only(bool setting)
{
	invite_only = setting;
}

void	Channel::set_topic_protection(bool setting)
{
	topic_protected = setting;
}

void	Channel::set_key(const std::string &new_key)
{
	key = new_key;
}

void	Channel::set_user_limit(int limit)
{
	user_limit = limit;
}

void	Channel::remove_user_limit()
{
	user_limit = -1; // -1은 사용자 제한이 없음을 의미합니다.
}

void	Channel::add_operator(const User &user)
{
	operator_user = &user;
}

void	Channel::remove_operator(const User &user)
{
	if (operator_user == &user)
		operator_user = NULL;
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
