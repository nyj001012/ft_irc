/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 01:27:55 by heshin            #+#    #+#             */
/*   Updated: 2023/11/18 02:20:28 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP
namespace IRC {
	bool is_special(const char);
	bool is_letter(const char c);
	bool is_digit(const char c);

	typedef bool(*predicate)(char); // signature for all valid template params
	template<predicate X, predicate Y>
		bool is_satisfy_all(const char c)
		{
			return X(c) && Y(c);
		}
	template<predicate X, predicate Y, predicate Z>
		bool is_satisfy_all(const char c)
		{
			return X(c) && Y(c) && Z(c);
		}
	template<predicate W, predicate X, predicate Y, predicate Z>
		bool is_satisfy_all(const char c)
		{
			return W(c) && X(c) && Y(c) && Z(c);
		}
	template<predicate X, predicate Y>
		bool is_satisfy_any(const char c)
		{
			return X(c) || Y(c);
		}
	template<predicate X, predicate Y, predicate Z>
		bool is_satisfy_any(const char c)
		{
			return X(c) || Y(c) || Z(c);
		}
	template<predicate W, predicate X, predicate Y, predicate Z>
		bool is_satisfy_any(const char c)
		{
			return W(c) || X(c) || Y(c) || Z(c);
		}
};

#include "../irc/IrcError.hpp"
#include "../irc/IrcCommand.hpp"
#endif
