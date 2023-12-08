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
using std::pair;
using std::set;

vector<Message> Message::create_start_from(const int fd) {
	vector<Message> vec;
	vec.push_back(Message());
	vec.back().recipients.push_back(make_pair(fd, string()));
	return vec;
}

Message& add_new_message(const vector<string>& new_messages, const string& prefix, const vector<pair<int, string> >& recipients, std::vector<Message>& messages) {

	vector<Message>::iterator iter;
	Message *target = NULL;
	for (iter = messages.begin(); iter != messages.end(); ++iter) {
		if (iter->recipients == recipients) {
			target = &*iter;
			break;
		}
	}
	if (target == NULL) {
		messages.push_back(Message());
		messages.back().recipients = recipients;
		target = &messages.back();
	}
	target->prefix = prefix;
	for (size_t i = 0; i < new_messages.size(); ++i) {
		target->contents.push_back(new_messages[i] + "\r\n");
	}
	return *target;
}

Message& add_new_message(const std::vector<std::string>& new_messages, const string& prefix, const std::pair<int, std::string> recipient, std::vector<Message>& messages) {
	vector<pair<int, string> > recipients;
	recipients.push_back(recipient);
	return add_new_message(new_messages, prefix, recipients, messages);
}

Message add_new_message(const vector<std::string>& new_message, const vector<int>& fds, std::vector<Message>& messages) {
	vector<pair<int, string> > recipients;
	for (size_t i = 0; i < fds.size(); ++i) {
		recipients.push_back(make_pair(fds[i], string()));
	}
	return add_new_message(new_message, string(), recipients, messages);
}

Message add_new_message(const vector<std::string>& new_message, const int fd, std::vector<Message>& messages) {
	vector<pair<int, string> > recipients;
	recipients.push_back(make_pair(fd, string()));
	return add_new_message(new_message, string(), recipients, messages);
}
vector<int> Message::get_all_fds(const vector<Message>& messages) {
	set<int> fds;
	for (size_t i = 0; i < messages.size(); ++i) {
		const Message& message = messages[i];
		for (size_t j = 0; j < message.recipients.size(); ++j) {
			fds.insert(message.recipients[j].first);
		}
	}
	return vector<int>(fds.begin(), fds.end());
}
void Message::add_recipient(std::pair<int, std::string> target) {
	recipients.push_back(target);
}
