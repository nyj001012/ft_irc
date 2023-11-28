/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 22:47:55 by heshin            #+#    #+#             */
/*   Updated: 2023/11/29 00:11:50 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Message.hpp"
#include <utility>
#include <set>

using std::vector;
using std::string;
using std::set;

void add_new_message(const vector<string>& new_messages, const vector<int>& fds, vector<Message>& messages) {
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
	target->contents.insert(target->contents.end(), 
			new_messages.begin(), new_messages.end());
}

void add_new_message(const vector<string>& new_messages, const int fd, vector<Message>& messages) {
	vector<int> fds;
	fds.push_back(fd);
	add_new_message(new_messages, fds, messages);
}

vector<int> Message::get_all_fds(const vector<Message>& messages) {
	set<int> fds;

	for (size_t i = 0; i < messages.size(); ++i) {
		fds.insert(messages[i].recipients_fd.begin(), messages[i].recipients_fd.end());
	}
	return vector<int>(fds.begin(), fds.end());
}

