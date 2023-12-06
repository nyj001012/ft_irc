/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcReply.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 01:11:20 by heshin            #+#    #+#             */
/*   Updated: 2023/12/05 21:17:45 by sejokim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_REPLY_HPP
# define IRC_REPLY_HPP
# include "../debug/Serializable.hpp"
# include <exception>

namespace IRC {
	struct Reply: public Serializable {

		enum Code {
			UnKnown = -1,
			RPL_WELCOME,
			RPL_NAMREPLY,
			RPL_ENDOFNAMES,
			RPL_TOPIC,
			RPL_INVITING,
			RPL_CHANNELMODEIS,
		} code;

		std::string prefix;
		std::vector<std::string> params;
		Reply();
		virtual ~Reply() throw();
		Reply(const Code, const std::string& prefix);
		Reply(const Code, const std::string& prefix, const std::vector<std::string>& params);
		std::string to_string() const;
		std::string get() const;
		
		virtual std::string _get_label() const;

		struct UnKnownError: public std::exception { 
			virtual const char* what() const throw();
		};
		struct InvalidNumbersOfParam: public std::exception { 
			virtual const char* what() const throw();
		};
	};
}
#endif

