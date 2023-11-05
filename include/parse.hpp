/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:43:05 by yena              #+#    #+#             */
/*   Updated: 2023/11/05 16:04:30 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_PARSE_PARSE_HPP_
#define FT_IRC_PARSE_PARSE_HPP_

#include <iostream>
#include "common.hpp"

bool isValidMessageFormat(std::string command);
bool isValidCommandWithOptions(std::string command);
bool isValidCommand(std::string command_part);
bool isExecutableCommand(std::string command_part);
bool isValidParams(std::string command_part);
bool isValidTrailing(std::string params);
bool isValidMiddle(std::string params);

#endif //FT_IRC_PARSE_PARSE_HPP_
