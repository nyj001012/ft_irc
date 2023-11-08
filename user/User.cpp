/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 02:43:18 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 02:43:18 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "../channel/Channel.hpp"
#include "../include/utils.hpp"
#include <netinet/in.h>
#include <stdexcept>
#include <vector>
#include <arpa/inet.h>

using std::string;
using std::vector;

Connection::Connection(): is_alive(false) {}
Connection::Connection(const struct sockaddr_storage* addr, const int socket_fd): is_alive(false), socket_fd(socket_fd) {
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

User::User() { }
User::User(const Connection connection, const string& nick)
	:connection(connection), nickname(nick), hostname("") { }

User::~User() { }

User::User(const User& other):
	connection(other.connection),
	nickname(other.nickname),
	hostname(other.hostname),
	joined_channels(other.joined_channels)
{
	
}

User& User::operator=(const User& other) {
	
	connection = other.connection;
	nickname = other.nickname;
	hostname = other.hostname;
	joined_channels = other.joined_channels;
	return *this;
}

bool User::is_available() const {
	return nickname.length() > 0 && connection.is_alive;
}

const string& User::get_nickname() const {
	return nickname;
}

const string& User::get_hostname() const {
	return hostname;
}

void User::add_channel(const Channel& channel) {
	if (!is_joined(channel.get_name())) {
		joined_channels.push_back(&channel);	
	}
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

bool User::operator==(const User& other) const {
	if (!(connection.operator==(other.connection)))
		return false;
	if (nickname != other.nickname || 
			hostname != other.hostname)
		return false;
	return true;
}

bool Connection::operator==(const Connection& other) const {
	if (is_alive != other.is_alive)
		return false;
	if (ip_family != other.ip_family)
		return false;
	if (address != other.address)
		return false;
	if (port != other.port)
		return false;
	if (socket_fd != other.socket_fd) {
		throw std::runtime_error("Different socket_fd for same connection");
	}
	return true;
}

bool operator==(const User& a, const User& b) {
	return a.operator==(b);
}

bool operator==(const Connection& a, const Connection& b) {
	return a.operator==(b);
}
