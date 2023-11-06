/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:43:05 by yena              #+#    #+#             */
/*   Updated: 2023/11/06 21:13:19 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_PARSE_PARSE_HPP_
#define FT_IRC_PARSE_PARSE_HPP_

#include <iostream>
#include "common.hpp"
#include "utils.hpp"
#include <iomanip>

#define SUCCESS true
#define FAIL false
#define SERVER_NAME_OR_NICK "server_name | nick"
#define USER "user"
#define HOST "host"
#define COMMAND "command"
#define PARAMS "params"

typedef struct s_token {
  std::string type;
  std::string value;
} t_token;

int parseMessageFormat(std::string command, bool is_debug, std::vector<t_token> &tokens);
int parseUserAndHost(std::string user_and_host, std::vector<t_token> &tokens);
int parseCommandWithOptions(std::string command, std::vector<t_token> &tokens);
int parseCommand(std::string command_part, std::vector<t_token> &tokens);
bool isExecutableCommand(std::string command_part);
int parseParams(std::string command_part, std::vector<t_token> &tokens);
int parseTrailing(std::string params, std::vector<t_token> &tokens);
int parseMiddle(std::string params, std::vector<t_token> &tokens);
void printTokens(std::vector<t_token> tokens);
size_t getLongestTokenType(std::vector<t_token> tokens);
size_t getLongestTokenValue(std::vector<t_token> tokens);

#endif //FT_IRC_PARSE_PARSE_HPP_
