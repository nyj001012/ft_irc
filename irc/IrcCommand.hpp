/* PASS ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:26:45 by heshin            #+#    #+#             */
/*   Updated: 2023/11/15 03:21:41 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_COMMAND
# define IRC_COMMAND
# include "../debug/Serializable.hpp"
# include <string>
# include <utility>

struct Command: public Serializable {

	typedef std::pair<int, int> range;

	virtual std::string _get_label() const;

	enum Type {
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
		//PONG,
		Unknown,
	} type;

	Command();
	Command(const std::string&);
	const std::pair<int, int> parameter_range() const;
	const char* to_string() const;
	struct UnSupported: public std::exception {
		virtual const char* what() const throw();
	};
};

bool operator==(const Command&, const Command::Type&);
bool operator==(const Command::Type&, const Command&);
bool operator!=(const Command&, const Command::Type&);
bool operator!=(const Command::Type&, const Command&);
#endif
