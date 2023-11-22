/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcReply.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 01:11:20 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 01:16:10 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_REPLY_HPP
# define IRC_REPLY_HPP
# include "../debug/Serializable.hpp"
# include <exception>

namespace IRC {
	struct Reply: public Serializable, std::exception {

		enum Code {
			UnKnown = -1,
			RPL_WELCOME,
		} code;

		Reply();
		virtual ~Reply() throw();
		Reply(const Code);
		const char* to_string() const;
		virtual std::string _get_label() const;
		virtual const char* what() const throw();

		struct UnKnownError: public std::exception { 
			virtual const char* what() const throw();
		};
	};
}
#endif

