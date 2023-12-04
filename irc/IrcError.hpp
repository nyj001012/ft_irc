/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:23:08 by heshin            #+#    #+#             */
/*   Updated: 2023/12/04 16:52:17 by sejokim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			ERR_UNKNOWNMODE,
			ERR_NOTOPIC,
		} code;

		Error();
		virtual ~Error() throw();
		Error(const Code);
		virtual std::string _get_label() const;
		virtual const char* what() const throw();

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
