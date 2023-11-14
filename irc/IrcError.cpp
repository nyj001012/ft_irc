/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 01:46:59 by heshin            #+#    #+#             */
/*   Updated: 2023/11/15 03:04:25 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include <string>
#define ERROR_STRINGIFY(name) # name

typedef IrcError::Code Code;
using std::string;

const char *all_errors[] = {
	ERROR_STRINGIFY(ERR_NEEDMOREPARAMS),
	ERROR_STRINGIFY(ERR_ALREADYREGISTRED),
};

IrcError::IrcError(): code(UnKnown) {}
IrcError::~IrcError() _NOEXCEPT{}
IrcError::IrcError(const Code code): code(code) {}

string IrcError::_get_label() const {
	return string("code: ") + to_string();
}

const char* IrcError::what() const throw() {
	return to_string();
}

const char* IrcError::to_string() const {
	if (code == UnKnown) {
		throw UnKnownError();
	}
	return all_errors[code];
}

bool operator==(const Code c, const IrcError& e) {
	return e.code == c;
}

bool operator==(const IrcError& e, const Code c) {
	return e.code == c;
}

bool operator!=(const IrcError& e, const Code c) {
	return !(e == c);
}

bool operator!=(const Code c, const IrcError& e) {
	return !(e == c);
}

const char* IrcError::UnKnownError::what() const throw() {
	return "UnKnownError";
}
