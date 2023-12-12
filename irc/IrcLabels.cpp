/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcLabels.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:42:18 by heshin            #+#    #+#             */
/*   Updated: 2023/11/25 23:42:18 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcLabels.hpp"
#include <string>

using IRC::ChannelLabel;

const char* ChannelLabel::NOT_ALLOWED_CHARS = "\32\7,";

std::string IRC::get_lower_name(const std::string& str) {
	std::string lower;
	for (size_t i = 0; i < str.size(); ++i) {
		char c = tolower(str[i]);
		if (c == '[' )
			c = '{';
		else if (c == ']')
			c = '}';
		else if (c == '\\')
			c = '|';
		else if (c == '~') 
			c = '^';
		lower.push_back(c);
	}
	return lower;
}
