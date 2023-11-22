/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcReply.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 01:15:10 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 01:20:46 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcReply.hpp"
#include <string>
#define REPLY_STRINGIFY(name) # name

typedef IRC::Reply Reply;
typedef Reply::Code Code;
using std::string;

const char *all_reply[] = {
	REPLY_STRINGIFY(RPL_WELCOME),
};

Reply::Reply(): code(UnKnown) {}
Reply::~Reply() throw(){}
Reply::Reply(const Code code): code(code) {}

string Reply::_get_label() const {
	return string("code: ") + to_string();
}

const char* Reply::what() const throw() {
	return to_string();
}

const char* Reply::to_string() const {
	if (code == UnKnown) {
		throw UnKnownError();
	}
	return all_reply[code];
}

const char* Reply::UnKnownError::what() const throw() {
	return "UnKnownError";
}
