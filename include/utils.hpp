/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:43:31 by yena              #+#    #+#             */
/*   Updated: 2023/11/06 18:41:16 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC__UTILS_HPP_
#define FT_IRC__UTILS_HPP_

#include "common.hpp"

void printError(std::string message);
bool isNumber(const char *string);
bool isValidPort(const char *port);
void skipChar(std::string &string, char c);

#endif //FT_IRC__UTILS_HPP_
