/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:43:05 by yena              #+#    #+#             */
/*   Updated: 2023/11/06 20:12:03 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_PARSE_PARSE_HPP_
#define FT_IRC_PARSE_PARSE_HPP_

#include <iostream>
#include "common.hpp"
#include "utils.hpp"

#define SUCCESS true
#define FAIL false

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

int parseMessageFormat(std::string command, bool is_debug, std::vector<t_token> &tokens);
int parseUserAndHost(std::string nick_and_host);
int parseCommandWithOptions(std::string command);
int parseCommand(std::string command_part);
bool isExecutableCommand(std::string command_part);
int parseParams(std::string command_part);
int parseTrailing(std::string params);
int parseMiddle(std::string params);

#endif //FT_IRC_PARSE_PARSE_HPP_
