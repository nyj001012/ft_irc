/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 22:47:55 by heshin            #+#    #+#             */
/*   Updated: 2023/11/29 03:46:27 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Message.hpp"
#include <utility>
#include <set>
#include <algorithm>

using std::vector;
using std::string;
using std::set;

vector<Message> Message::create_start_from(const int fd) {
	vector<Message> vec;
	vec.push_back(Message());
	vec.back().recipients_fd.push_back(fd);
	return vec;
}

Message& add_new_message(const vector<string>& new_messages, const vector<int>& fds, vector<Message>& messages) {
	vector<Message>::iterator iter;
	Message *target = NULL;
	for (iter = messages.begin(); iter != messages.end(); ++iter) {
		if (iter->recipients_fd == fds) {
			target = &*iter;
			break;
		}
	}
	if (target == NULL) {
		messages.push_back(Message());
		messages.back().recipients_fd = fds;
		target = &messages.back();
	}
	for (size_t i = 0; i < new_messages.size(); ++i) {
		target->contents.push_back(new_messages[i] + "\r\n");
	}
	return *target;
}

Message& add_new_message(const vector<string>& new_messages, const int fd, vector<Message>& messages) {
	vector<int> fds;
	fds.push_back(fd);
	return add_new_message(new_messages, fds, messages);
}

void Message::remove_fd(const int fd) {
	size_t i = 0;
	while (i < recipients_fd.size()) {
		if (recipients_fd[i] == fd)
			recipients_fd.erase(recipients_fd.begin() + i);
		else 
			++i;
	}
}

vector<int> Message::get_all_fds(const vector<Message>& messages) {
	set<int> fds;

	for (size_t i = 0; i < messages.size(); ++i) {
		fds.insert(messages[i].recipients_fd.begin(), messages[i].recipients_fd.end());
	}
	return vector<int>(fds.begin(), fds.end());
}

