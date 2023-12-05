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
	ERROR_STRINGIFY(ERR_NOSUCHCHANNEL),
	ERROR_STRINGIFY(ERR_NOTONCHANNEL),
	ERROR_STRINGIFY(ERR_CANNOTSENDTOCHAN),
	ERROR_STRINGIFY(ERR_NORECIPIENT),
	ERROR_STRINGIFY(ERR_NOSUCHNICK),
	ERROR_STRINGIFY(ERR_NOTEXTTOSEND),
	ERROR_STRINGIFY(ERR_UNKNOWNMODE),
	ERROR_STRINGIFY(ERR_NOTOPIC),
	ERROR_STRINGIFY(ERR_USERONCHANNEL),
	ERROR_STRINGIFY(ERR_NOORIGIN),
};

Error::Error(): code(UnKnown) {}
Error::~Error() throw(){}
Error::Error(const Code code): code(code) {}

string Error::_get_label() const {
	return string("code: ") + all_errors[code];
}

const char* Error::what() const throw() {
	if (code == UnKnown) {
		return "UnKnwon";
	}
	return all_errors[code];
}

string Error::get_message() const {
	string descripton;
	return string(what()) + " :" + descripton;
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
