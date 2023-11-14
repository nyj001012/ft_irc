/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:23:08 by heshin            #+#    #+#             */
/*   Updated: 2023/11/15 02:44:21 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_ERROR_HPP
# define IRC_ERROR_HPP
# include "../debug/Serializable.hpp"
# include <exception>

struct IrcError: public Serializable, std::exception {

	enum Code {
		UnKnown = -1,
		ERR_NEEDMOREPARAMS,
		ERR_ALREADYREGISTRED,
	} code;

	IrcError();
	virtual ~IrcError() _NOEXCEPT;
	IrcError(const Code);
	const char* to_string() const;
	virtual std::string _get_label() const;
	virtual const char* what() const throw();

	struct UnKnownError: public std::exception { 
		virtual const char* what() const throw();
	};
};

bool operator==(const IrcError::Code, const IrcError&);
bool operator==(const IrcError&, const IrcError::Code);
bool operator!=(const IrcError::Code, const IrcError&);
bool operator!=(const IrcError&, const IrcError::Code);
#endif
