/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:43:05 by yena              #+#    #+#             */
/*   Updated: 2023/11/06 19:56:20 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_PARSE_PARSE_HPP_
#define FT_IRC_PARSE_PARSE_HPP_

#include <iostream>
#include "common.hpp"
#include "utils.hpp"

#define SUCCESS 0
#define FAIL 1

typedef enum e_token_type {
  SERVER_NAME,
  NICK,
  USER,
  HOST,
  COMMAND,
  PARAMS,
  SPACE,
} t_token_type;

typedef struct s_token {
  t_token_type type;
  std::string value;
} t_token;

bool isValidMessageFormat(std::string command, bool is_debug);
bool isValidUserAndHost(std::string nick_and_host);
bool isValidCommandWithOptions(std::string command);
bool isValidCommand(std::string command_part);
bool isExecutableCommand(std::string command_part);
bool isValidParams(std::string command_part);
bool isValidTrailing(std::string params);
bool isValidMiddle(std::string params);

#endif //FT_IRC_PARSE_PARSE_HPP_
