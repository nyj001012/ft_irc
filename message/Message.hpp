/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 00:14:43 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 19:12:35 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP
# include <string>
# include <vector>
# include <exception>

struct Command {
	enum Type{
		PASS = 0,
		USER,
		NICK,
		QUIT,
		JOIN,
		PART,
		MODE,
		TOPIC,
		INVITE,
		PRIVMSG,
		//PING,
		//PONG
	} type;

	Command(const std::string&);
	Command();

	std::string type_string() const;
	struct UnSupported: public std::exception {
		virtual const char* what() const throw();
	};
};

struct Message {

	Command command;
	std::string prefix;
	std::vector<std::string> params;

	struct ParsingFail: public std::exception {
		 virtual const char* what() const throw();
	};
};
#endif
