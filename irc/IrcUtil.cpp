/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUtil.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 01:26:23 by heshin            #+#    #+#             */
/*   Updated: 2023/11/18 02:20:16 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Irc.hpp"
#include <cstring>
#include <ctype.h>

bool IRC::is_letter(const char c) {
 return isalpha(c) != 0;
}

bool IRC::is_digit(const char c) {
 return isdigit(c) != 0;
}

bool IRC::is_special(const char c) {
 return strchr("[,],\\`_^{|}", c) != NULL;
}
