#include "IrcError.hpp"
#include <sstream>
#include <string>
#define ERROR_STRINGIFY(name) # name

typedef IRC::Error Error;
typedef Error::Code Code;
using std::string;
using std::pair;
using std::make_pair;

const pair<const char *, int> all_errors[] = {
	make_pair(ERROR_STRINGIFY(ERR_NEEDMOREPARAMS), 461),
	make_pair(ERROR_STRINGIFY(ERR_ALREADYREGISTRED), 462),
	make_pair(ERROR_STRINGIFY(ERR_NONICKNAMEGIVEN), 431),
	make_pair(ERROR_STRINGIFY(ERR_ERRONEUSNICKNAME), 432),
	make_pair(ERROR_STRINGIFY(ERR_NICKCOLLISION), 436),
	make_pair(ERROR_STRINGIFY(ERR_CHANOPRIVSNEEDED), 482),
	make_pair(ERROR_STRINGIFY(ERR_BADCHANNELKEY), 475),
	make_pair(ERROR_STRINGIFY(ERR_NOSUCHCHANNEL), 403),
	make_pair(ERROR_STRINGIFY(ERR_NOTONCHANNEL), 442),
	make_pair(ERROR_STRINGIFY(ERR_CANNOTSENDTOCHAN), 404),
	make_pair(ERROR_STRINGIFY(ERR_NORECIPIENT), 411),
	make_pair(ERROR_STRINGIFY(ERR_NOSUCHNICK), 401),
	make_pair(ERROR_STRINGIFY(ERR_NOTEXTTOSEND), 412),
	make_pair(ERROR_STRINGIFY(ERR_UNKNOWNMODE), 472),
	make_pair(ERROR_STRINGIFY(ERR_NOTOPIC), 331),
	make_pair(ERROR_STRINGIFY(ERR_USERONCHANNEL), 443),
	make_pair(ERROR_STRINGIFY(ERR_NOORIGIN), 409),
	make_pair(ERROR_STRINGIFY(ERR_UNKNOWNCOMMAND), 421)
};

Error::Error(): code(UnKnown) {}
Error::~Error() throw(){}
Error::Error(const Code code): code(code) {}

string Error::_get_label() const {
	return string("code: ") + all_errors[code].first;
}

const char* Error::what() const throw() {
	if (code == UnKnown) {
		return "UnKnwon";
	}
	return all_errors[code].first;
}

string Error::get_message() const {
	std::stringstream ss;
	ss << all_errors[code].second;
	ss << " :" << all_errors[code].first;
	return ss.str();
}

string Error::_get_message(const Error & error) {
	return error.get_message();
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
