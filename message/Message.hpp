/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 00:14:43 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 00:14:43 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP
# include <string>
# include <vector>
# include <exception>

struct Command {
	enum Type{
		NICK = 0,
		INVITE,
		TOPIC,
		MODE
	} type;

	Command(const std::string&);
	Command();

	std::string type_string() const;
	struct UnSupported: public std::exception {
		virtual const char* what();
	};
};

struct Message {

	std::string prefix;
	Command command;
	std::vector<std::string> params;

	Message(const std::string&);

	struct ParsingFail: public std::exception {
		 virtual const char* what();
	};
};
#endif
