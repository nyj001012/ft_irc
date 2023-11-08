/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserData.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 13:19:44 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 13:19:44 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserData.hpp"
#include "../user/User.hpp"
#include <utility>

using std::string;
using std::list;
using std::pair;
using std::make_pair;
using std::map;

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

User& UserData::get_user(const string& nickname) const {
	map<string, list<User>::iterator>::const_iterator found = user_nick_map.find(nickname);
	if (found == user_nick_map.end())
		throw UserNotExist();
	return *found->second;
}

void UserData::add_user(const User& user) {
	users.push_back(user);	
	list<User>::iterator iter = users.end();
	--iter;	
	pair<string, list<User>::iterator> p = make_pair(user.get_nickname(), iter);
	if (user_nick_map.insert(p).second) {
		users.pop_back();
		throw UserAlreadyExist();
	}
}

void UserData::delete_user(const User& user) {

	if (user.get_nickname().length() != 0 &&
			is_user_exist(user.get_nickname())) {
		map<string, list<User>::iterator>::iterator found = 
			user_nick_map.find(user.get_nickname());
		user_nick_map.erase(found);
		users.erase(found->second);
	}
	else {
		users.remove(user);
	}
}

const char* UserData::UserNotExist::what() {
	return "User not exist";
}

const char* UserData::UserAlreadyExist::what() {
	return "User already exist";
}
