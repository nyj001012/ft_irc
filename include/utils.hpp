/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:43:31 by yena              #+#    #+#             */
/*   Updated: 2023/11/28 23:33:19 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC__UTILS_HPP_
#define FT_IRC__UTILS_HPP_

#include "common.hpp"
#include <vector>
#include <string>

void printError(std::string message);
bool isNumber(const char* string);
bool isValidPort(const char* port);
void skipChar(std::string& string, char c);

void *get_in_addr(const struct sockaddr *sa);
std::vector<std::string> split_string(const std::string&);
std::vector<std::string> split_string(const std::string&, const char);
std::vector<std::string> strs_to_vector(const std::string&);
std::vector<std::string> strs_to_vector(const std::string&, const std::string&);
std::vector<std::string> strs_to_vector(const std::string&,const std::string&, const std::string&);
std::vector<std::string> strs_to_vector(const std::string&,const std::string&, const std::string&, const std::string&);
#endif //FT_IRC__UTILS_HPP_
