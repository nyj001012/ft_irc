/* PASS ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:26:45 by heshin            #+#    #+#             */
/*   Updated: 2023/11/29 23:03:29 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_COMMAND
# define IRC_COMMAND
# include "../debug/Serializable.hpp"
# include <string>
# include <utility>

namespace IRC {
	struct Command: public Serializable {
		typedef std::pair<int, int> range;
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
			KICK,
			PRIVMSG,
			//PING,
			//PONG,
			Unknown,
		} type;

		Command();
		Command(const std::string&);
		const std::pair<int, int> parameter_range() const;

		virtual std::string _get_label() const;
		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 

		struct UnSupported: public std::exception {
			virtual const char* what() const throw();
		};
	};
}
bool operator==(const IRC::Command&, const IRC::Command::Type&);
bool operator==(const IRC::Command::Type&, const IRC::Command&);
bool operator!=(const IRC::Command&, const IRC::Command::Type&);
bool operator!=(const IRC::Command::Type&, const IRC::Command&);
#endif
