/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:42:07 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 18:42:07 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "../user/User.hpp"
#include <algorithm>
#include <iterator>

using std::string;
using std::vector;

Channel::Channel(): operator_user(NULL) {}
Channel::~Channel() { }
Channel::Channel(const string& name, const User& operator_user)
	:name(name),
	operator_user(&operator_user)
{ }

const string& Channel::get_name() const {
	return name;
}

const string& Channel::get_topic() const {
	return topic;
}

vector<const User*> Channel::get_users() const {
	return users;
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

bool Channel::operator==(const Channel& other) const {
	if (name != other.name || topic != other.topic)
		return false;
	if (users != other.users)
		return false;

	return true;
}

bool operator==(const Channel& a, const Channel& b) {
	return a.operator==(b);
}

Channel::Channel(const Channel& other) {
	(void)other;
}

Channel& Channel::operator=(const Channel& other) {
	(void)other;
	return *this;
}

const char* Channel::AlreadyJoined::what() {
	return "Already joined to channel";
}

const char* Channel::NoPermission::what() {
	return "No permission";
}
