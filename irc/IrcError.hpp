#ifndef IRC_ERROR_HPP
# define IRC_ERROR_HPP
# include "../debug/Serializable.hpp"
# include <exception>

namespace IRC {
	struct Error: public Serializable, std::exception {

		enum Code {
			UnKnown = -1,
			ERR_NEEDMOREPARAMS,
			ERR_ALREADYREGISTRED,
			ERR_NONICKNAMEGIVEN,
			ERR_ERRONEUSNICKNAME,
			ERR_NICKNAMEINUSE,
			ERR_CHANOPRIVSNEEDED,
			ERR_BADCHANNELKEY,
			ERR_NOSUCHCHANNEL,
			ERR_NOTONCHANNEL,
			ERR_CANNOTSENDTOCHAN,
			ERR_NORECIPIENT,
			ERR_NOSUCHNICK,
			ERR_NOTEXTTOSEND,
			ERR_UNKNOWNMODE,
			ERR_NOTOPIC,
			ERR_USERONCHANNEL,
			ERR_NOORIGIN,
			ERR_UNKNOWNCOMMAND,
			ERR_CHANNELISFULL,
			ERR_INVITEONLYCHAN,
		} code;

		Error();
		virtual ~Error() throw();
		Error(const Code);
		virtual std::string _get_label() const;
		virtual const char* what() const throw();
		static std::string _get_message(const Error&);
		std::string get_message() const;

		struct UnKnownError: public std::exception { 
			virtual const char* what() const throw();
		};
	};
}
bool operator==(const IRC::Error::Code, const IRC::Error&);
bool operator==(const IRC::Error&, const IRC::Error::Code);
bool operator!=(const IRC::Error::Code, const IRC::Error&);
bool operator!=(const IRC::Error&, const IRC::Error::Code);
#endif
