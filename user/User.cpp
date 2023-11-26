/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 02:43:18 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 01:10:32 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "../channel/Channel.hpp"
#include "../include/utils.hpp"
#include "../include/json.hpp"
#include "../debug/Serializable.hpp"
#include <algorithm>
#include <netinet/in.h>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>
#include <arpa/inet.h>
#include <sstream>

using std::string;
using std::vector;
using std::ostream;
using std::stringstream;
using std::ostream;
using std::pair;
using std::make_pair;

typedef std::pair<string, const Serializable*> KeyValue;

Connection::Connection(): is_alive(true) {}
Connection::Connection(const struct sockaddr_storage* addr, const int socket_fd): 
	is_alive(false), socket_fd(socket_fd), password() {
	char buffer[INET6_ADDRSTRLEN];
	switch (addr->ss_family) {
		case (AF_INET):
			ip_family = V4;
			port = htons(((const struct sockaddr_in*)addr)->sin_port);
			break;
		case (AF_INET6):
			ip_family = V6;
			port = htons(((const struct sockaddr_in6*)addr)->sin6_port);
			break;
		default:
			return;
	}
	if (inet_ntop(addr->ss_family, 
				get_in_addr((const struct sockaddr*)addr), 
				buffer, sizeof(buffer)) != NULL) {
		address = string(buffer);
		is_alive = true;
	} 
	else 
		return;
}
string User::Info::get_id() const {
	return nick_name + '!' + user_name + '@' + host_name;
}

bool User::Info::is_equal(const User::Info& other) const {
	if (nick_name != other.nick_name)
		return false;
	if (user_name != other.user_name)
		return false;
	if (host_name != other.host_name)
		return false;
	if (server_name != other.server_name)
		return false;
	if (real_name != other.real_name)
		return false;
	return true;
}

User::User() { }
User::User(const Connection connection, const Info& info)
	:connection(connection), info(info){ }

User::~User() { }

User::User(const User& other)
{
	*this = other;	
}

User& User::operator=(const User& other) {
	
	connection = other.connection;
	info = other.info;
	joined_channels = other.joined_channels;
	return *this;
}

bool User::is_available() const {
	return connection.is_alive;
}

const string& User::get_nickname() const {
	return info.nick_name;
}

const Connection& User::get_connection() const {
	return connection;
}

const User::Info& User::get_info() const {
	return info;
}

void User::add_channel(const Channel& channel) {
	vector<const Channel*>::iterator found;
	found = std::find(joined_channels.begin(), joined_channels.end(), &channel);
	if (found == joined_channels.end())
		joined_channels.push_back(&channel);	
}

void User::remove_channel(const Channel& channel) {

	std::vector<const Channel*>::iterator iter;
	for (iter = joined_channels.begin(); iter != joined_channels.end(); ++iter) {
		if (*iter == &channel) {
			break;
		}
	}
	if (iter == joined_channels.end())
		return ;
	joined_channels.erase(iter);
}

void User::remove_channel(const string& name) {
	std::vector<const Channel*>::iterator iter;
	for (iter = joined_channels.begin(); iter != joined_channels.end(); ++iter) {
		if (name == (*iter)->get_name())
			break;
	}
	if (iter == joined_channels.end())
		return ;
	joined_channels.erase(iter);
}

bool User::is_joined(const string& channel_name) const {
	vector<const Channel*>::const_iterator iter;
	for (iter = joined_channels.begin(); iter != joined_channels.end(); ++iter) {
		if ((*iter)->get_name() == channel_name)
			return true;
	}
	return false;
}

vector<const Channel*> User::get_all_channels() const {
	return joined_channels;
}

bool User::is_equal(const User& other) const {
	if (!(connection.is_equal(other.connection)))
		return false;
	if (!(info == other.info))
		return false;
	return true;
}

bool Connection::is_equal(const Connection& other) const {
	if (is_alive != other.is_alive)
		return false;
	if (ip_family != other.ip_family)
		return false;
	if (address != other.address)
		return false;
	if (port != other.port)
		return false;
	if (socket_fd != other.socket_fd)
		return false;
	return true;
}

bool operator==(const User& a, const User& b) {
	return a.is_equal(b);
}

bool operator==(const Connection& a, const Connection& b) {
	return a.is_equal(b);
}

bool operator==(const User::Info& a, const User::Info& b) {
	return a.is_equal(b);
}

// Serializable

string Connection::_get_label() const {
	stringstream ss;
	ss << "socket=" << socket_fd;
	if (is_alive)
		ss << "(alive)";
	else 
		ss << "(not alive)";
	return ss.str();
} 

ostream& Connection::_add_to_serialization(ostream& os, const int _d) const {
	
	_json(os, "address", ':', address, ',');
	_json(os, "port", ':', port, ',');
	_json(os, "is_alive", ':', is_alive, ',');
	_json(os, "socket_fd", ':', socket_fd, ',');
	_json(os, "password", ':', password);
	(void)_d;
	return os;
}

string User::Info::_get_label() const {
	if (host_name.empty())
		return nick_name;
	return nick_name + "@" + host_name;
}

string User::_get_label() const {
	return info._get_label();
}

vector<KeyValue> User::_get_children() const {
	vector<KeyValue> v;
	v.push_back(make_pair("info", &this->info));
	v.push_back(make_pair("connection", &this->connection));		
	return v;
}

ostream& User::Info::_add_to_serialization(ostream& os, const int depth) const {
	(void)depth;
	_json(os, "nick", ':', nick_name, ',');
	_json(os, "user", ':', user_name, ',');
	_json(os, "host", ':', host_name, ',');
	_json(os, "server", ':', server_name, ',');
	_json(os, "real name", ':', real_name);
	return os;
}

ostream& User::_add_to_serialization(ostream& os, const int depth) const {

	if (depth > 0) {
		vector<const Serializable*> vec(joined_channels.begin(), joined_channels.end());
		_json(os, "channel", ':') << ::_serialize(vec, depth - 1);
	}
	else {
		_json(os, "number of joined channels", ':', joined_channels.size());
	}
	return os;
}
