/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 01:46:59 by heshin            #+#    #+#             */
/*   Updated: 2023/11/22 23:35:48 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include <string>
#define ERROR_STRINGIFY(name) # name

typedef IRC::Error Error;
typedef Error::Code Code;
using std::string;

const char *all_errors[] = {
	ERROR_STRINGIFY(ERR_NEEDMOREPARAMS),
	ERROR_STRINGIFY(ERR_ALREADYREGISTRED),
	ERROR_STRINGIFY(ERR_NONICKNAMEGIVEN),
	ERROR_STRINGIFY(ERR_ERRONEUSNICKNAME),
	ERROR_STRINGIFY(ERR_NICKCOLLISION),
	ERROR_STRINGIFY(ERR_CHANOPRIVSNEEDED),
	ERROR_STRINGIFY(ERR_BADCHANNELKEY),
};

Error::Error(): code(UnKnown) {}
Error::~Error() throw(){}
Error::Error(const Code code): code(code) {}

string Error::_get_label() const {
	return string("code: ") + to_string();
}

const char* Error::what() const throw() {
	return to_string();
}

const char* Error::to_string() const {
	if (code == UnKnown) {
		throw UnKnownError();
	}
	return all_errors[code];
}

bool operator==(const Code c, const Error& e) {
	return e.code == c;
}

bool operator==(const Error& e, const Code c) {
	return e.code == c;
}

bool operator!=(const Error& e, const Code c) {
	return !(e == c);
}

bool operator!=(const Code c, const Error& e) {
	return !(e == c);
}

const char* Error::UnKnownError::what() const throw() {
	return "UnKnownError";
}
