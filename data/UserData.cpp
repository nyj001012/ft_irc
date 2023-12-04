/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserData.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 13:19:44 by heshin            #+#    #+#             */
/*   Updated: 2023/12/04 13:14:09 by sejokim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserData.hpp"
#include "../user/User.hpp"
#include "../include/json.hpp"
#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

using std::string;
using std::list;
using std::pair;
using std::make_pair;
using std::map;
using std::stringstream;
using std::vector;
using std::ostream;

typedef pair<string, const Serializable*> KeyValue;

UserData::UserData() {}
UserData::~UserData() {
	//TODO: Disconnect
}

UserData& UserData::get_storage() {
	static UserData storage;

	return storage;
}

bool UserData::is_user_exist(const string& nickname) const {
	return user_nick_map.find(nickname) != user_nick_map.end(); 
}

bool UserData::is_user_exist(const Connection& connection) const {
	return user_socket_map.find(connection.socket_fd) != user_socket_map.end();
}

User& UserData::get_user(const string& nickname) const {
	map<string, list<User>::iterator>::const_iterator found = user_nick_map.find(nickname);
	if (found == user_nick_map.end())
		throw UserNotExist();
	return *found->second;
}

User& UserData::get_user(const Connection& connection) const {

	map<int, list<User>::iterator>::const_iterator found = user_socket_map.find(connection.socket_fd);
	if (found == user_socket_map.end())
		throw UserNotExist();
	return *found->second;
}

User& UserData::create_user(const Connection connection, const User::Info& info) {
	typedef list<User>::iterator UserIter;
	users.push_back(User(connection, info));	
	list<User>::iterator iter = users.end();
	--iter;	

	pair<int, UserIter> socket_pair = make_pair(iter->get_connection().socket_fd, iter);
	pair<map<int, UserIter>::iterator, bool> socket_inserted = user_socket_map.insert(socket_pair);
	if (!socket_inserted.second) {
		users.pop_back();
		throw UserAlreadyExist();
	}
	pair<string, UserIter> nick_pair = make_pair(iter->get_nickname(), iter);
	if (!user_nick_map.insert(nick_pair).second) {
		users.pop_back();
		user_socket_map.erase(socket_inserted.first);
		throw UserAlreadyExist();
	}
	return *iter;
}

void UserData::delete_user(const User& user) {

	typedef list<User>::iterator UserIter;
	if (user.get_nickname().length() != 0 &&
			is_user_exist(user.get_nickname())) {
		map<int, UserIter>::iterator socket_found = user_socket_map.find(user.get_connection().socket_fd);;
		map<string, UserIter>::iterator nick_found = user_nick_map.find(user.get_nickname());
		user_socket_map.erase(socket_found);
		user_nick_map.erase(nick_found);
		users.erase(nick_found->second);
	}
	else {
		users.remove(user);
	}
}

bool UserData::is_pedding_user_exist(const Connection& c) const {
	map<int, UserTask>::const_iterator found = pendding_users.find(c.socket_fd);
	if (found == pendding_users.end())
		return false;
	if (!(found->second.get_connection() == c)) {
		// TODO: Server error
		map<int, UserTask>& m = const_cast<map<int, UserTask>&>(pendding_users);
		m.erase(c.socket_fd);
		return false;
	}
	return true;
}

bool UserData::is_duplicated(const string& nick_name) const {
	if (is_user_exist(nick_name))
		return true;
	map<int, UserTask>::const_iterator iter;
	for (iter = pendding_users.begin(); iter != pendding_users.end(); ++iter) {
		if (iter->second.info.nick_name == nick_name)
			return true;
	}
	return false;
}

void UserData::add_pendding_user(const UserTask& task) {
	pendding_users.insert(make_pair(task.get_connection().socket_fd, task));
}

const UserTask& UserData::update_task(const UserTask& new_task) {
	UserTask& exist = pendding_users.at(new_task.get_connection().socket_fd);
	if (new_task.get_command() == Command::USER &&
			exist.info.nick_name.empty()) {
		return exist;
	}
	else 
		return exist.add_next(new_task);
}

void UserData::remove_task(const Connection& c) {
	pendding_users.erase(c.socket_fd);
}

// Serializable

string UserData::_get_label() const {
	stringstream ss;
	ss << "User Data(number of users=" << users.size() << ')';
	return ss.str();
}

ostream& UserData::_add_to_serialization(ostream& os, const int depth) const {

	typedef list<User>::iterator UserIter;	
	typedef list<User>::const_iterator UserConstIter;	
	_json(os, "number of users", ':', users.size(), ',');
	_json(os, "number of pendding users", ':', pendding_users.size());
	if (depth > 0 && users.size() > 0) {
		os << ',';
		vector<const Serializable *>vec;
		for (UserConstIter iter = users.begin(); iter != users.end(); ++iter) {
			vec.push_back(&*iter);
		}
		_json(os, "users", ':');
		os << ::_serialize(vec, depth - 1);
		if (user_nick_map.size() > 0) {
			os << ',';
			map<string, const Serializable *> m;
			map<string, UserIter>::const_iterator iter;
			for (iter = user_nick_map.begin(); iter != user_nick_map.end(); ++iter) {
				m.insert(make_pair(iter->first, &*iter->second));
			}
			_json(os, "user for nickname", ':');
			os << ::_serialize(m, depth - 1);
		}
		if (user_socket_map.size() > 0) {
			os << ',';
			map<string, const Serializable *> m;
			map<int, UserIter>::const_iterator iter;
			for (iter = user_socket_map.begin(); iter != user_socket_map.end(); ++iter) {
				stringstream ss;
				ss << iter->first;
				m.insert(make_pair(ss.str(), &*iter->second));
			}
			_json(os, "user for socket", ':');
			os << ::_serialize(m, depth - 1);
		}
	}
	if (depth > 0 && pendding_users.size() > 0) {
		os << ',';	
		vector<const Serializable *>vec;
		map<int, UserTask>::const_iterator iter;
		for (iter = pendding_users.begin(); iter != pendding_users.end(); ++iter) {
			vec.push_back(&iter->second);
		}
		_json(os, "pendding users", ':');
		os << ::_serialize(vec, depth - 1);
	}
	return os;
}

const char* UserData::UserNotExist::what() const throw() {
	return "User not exist";
}

const char* UserData::UserAlreadyExist::what() const throw() {
	return "User already exist";
}

