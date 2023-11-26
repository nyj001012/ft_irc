/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:23:08 by heshin            #+#    #+#             */
/*   Updated: 2023/11/22 23:35:32 by heshin           ###   ########.fr       */
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
