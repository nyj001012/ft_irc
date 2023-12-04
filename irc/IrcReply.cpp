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
#include <stdexcept>
#include <string>
#define REPLY_STRINGIFY(name) # name

typedef IRC::Reply Reply;
typedef Reply::Code Code;
using std::string;
using std::vector;

size_t get_minimum_number_of_params(const Code);

const char *all_reply[] = {
	REPLY_STRINGIFY(RPL_WELCOME),
	REPLY_STRINGIFY(RPL_NAMREPLY),
	REPLY_STRINGIFY(RPL_ENDOFNAMES),
	REPLY_STRINGIFY(RPL_TOPIC),
	REPLY_STRINGIFY(RPL_INVITING),
};

string get_code(const Code code) {
	switch (code) {
		case Reply::RPL_WELCOME:
			return "001";
		case Reply::RPL_NAMREPLY:
			return "355";
		case Reply::RPL_ENDOFNAMES:
			return "366";
		case Reply::RPL_TOPIC:
			return "332";
		case Reply::RPL_INVITING:
			return "341";
		default:
			throw Reply::UnKnownError();
	}
}


Reply::Reply(): code(UnKnown) {}
Reply::~Reply() throw(){}
Reply::Reply(const Code code, const string& prefix)
	: code(code),
	prefix(prefix)
{ 
	if (get_minimum_number_of_params(code) != 0)
		throw InvalidNumbersOfParam();
}

Reply::Reply(const Code code, const string& prefix, const vector<string>& params)
	: code(code),
	prefix(prefix),
	params(params)
{ 
	if (get_minimum_number_of_params(code) > params.size())
		throw InvalidNumbersOfParam();
}

string Reply::to_string() const {
	string reply;
	if (!prefix.empty())
		reply += ":" + prefix + ' ';
	reply += get_code(code);
	if (params.empty())
		return reply;
	switch (code) {
		case RPL_NAMREPLY:
		case RPL_ENDOFNAMES:
			reply += " " + params[0] + " = " + params[1] + " :";
			if (code == RPL_NAMREPLY) {
				for (size_t i = 2; i < params.size(); ++i) {
					reply += params[i];
					if (i + 1 != params.size())
						reply += ' ';
				}
			}
			else
				reply += "End of Names list";
			break;
		default:
			for (size_t i = 0; i < params.size(); ++i) {
				reply += (i + 1 == params.size() && params[i].find(' ') != string::npos) ? " :" : " ";
				reply += params[i];
			}
	}
	return reply;
}

string Reply::_get_label() const {
	if (code == UnKnown) {
		throw UnKnownError();
	}
	return string("code: ") + all_reply[code];
}

const char* Reply::UnKnownError::what() const throw() {
	return "UnKnownError";
}

const char* Reply::InvalidNumbersOfParam::what() const throw() {
	return "InvalidNumbersOfParam";
}

size_t get_minimum_number_of_params(const Code code) {
	switch (code) {
		case (Reply::RPL_NAMREPLY): 
		case(Reply::RPL_ENDOFNAMES):
			return 2;
			break;
		case(Reply::RPL_TOPIC):
			return 1;
		default:
			return 0;
	}
}
