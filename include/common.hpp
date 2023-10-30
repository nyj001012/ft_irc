/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:09:12 by yena              #+#    #+#             */
/*   Updated: 2023/10/30 22:47:14 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_INCLUDE_COMMON_HPP_
#define FT_IRC_INCLUDE_COMMON_HPP_

#include <iostream>

#define F_BLACK "\033[0;30m"
#define F_RED "\033[0;31m"
#define F_GREEN "\033[0;32m"
#define F_YELLOW "\033[0;33m"
#define F_BLUE "\033[0;34m"
#define F_MAGENTA "\033[0;35m"
#define F_CYAN "\033[0;36m"
#define F_WHITE "\033[0;37m"
#define FB_DEFAULT "\033[0m"

extern bool is_debug;
extern int server_socket;

#endif //FT_IRC_INCLUDE_COMMON_HPP_
