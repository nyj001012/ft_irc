/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:43:31 by yena              #+#    #+#             */
/*   Updated: 2023/10/30 22:06:46 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC__UTILS_HPP_
#define FT_IRC__UTILS_HPP_

#include "common.hpp"
#include <vector>
#include <string>

void printError(std::string message);
bool isNumber(const char *str);
bool isValidPort(const char *port);

void *get_in_addr(const struct sockaddr *sa);
#endif //FT_IRC__UTILS_HPP_
