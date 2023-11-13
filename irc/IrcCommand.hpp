/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:26:45 by heshin            #+#    #+#             */
/*   Updated: 2023/11/13 04:26:45 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_COMMAND
# define IRC_COMMAND
# include <string>
# include <utility>

struct Command {

	typedef std::pair<int, int> range;

	enum Type {
		Unknown,
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
