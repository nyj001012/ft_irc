/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:43:05 by yena              #+#    #+#             */
/*   Updated: 2023/11/06 20:55:55 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.hpp"

/*
** Command format **

<message>  ::= [':' <options> <SPACE> ] <command> <params> <crlf>
<options>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
<command>  ::= <letter> { <letter> } | <number> <number> <number>
<SPACE>    ::= ' ' { ' ' }
<params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]

<middle>   ::= <Any *non-empty* sequence of octets not including SPACE
or NUL or CR or LF, the first of which may not be ':'>
<trailing> ::= <Any, possibly *empty*, sequence of octets not including
    NUL or CR or LF>

<crlf>     ::= CR LF
*/

/**
 * 유효한 명령어인지 확인하는 함수
 * @param command 확인할 명령어
 * @param is_debug 디버그 모드인지 확인하는 변수
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 명령어면 SUCCESS, 아니면 FAIL
 */
int parseMessageFormat(std::string command, bool is_debug, std::vector<t_token> &tokens) {
  bool result;
  if (command.empty() || command[command.length() - 1] != '\n')
    result = FAIL;
  command.erase(command.length() - 1);
  if (command[0] == ':')
    result = parseCommandWithOptions(command, tokens);
  else
    result = parseCommand(command, tokens);
  if (is_debug)
    printTokens(tokens);
  return result;
}

/**
 * 유효한 options가 있는 명령어인지 확인하는 함수
 * options는 <servername> | <nick> [ '!' <user> ] [ '@' <host> ] 형식이어야 한다.
 * @param command 확인할 명령어, command[0] == ':'
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 options면 SUCCESS, 아니면 FAIL
 */
int parseCommandWithOptions(std::string command, std::vector<t_token> &tokens) {
  size_t pos = command.find(' ');
  if (pos == std::string::npos)
    return FAIL;
  std::string options = command.substr(1, pos - 1);
  if (options.empty())
    return FAIL;
  if (!parseUserAndHost(options, tokens)) {
    return FAIL;
  }
  skipChar(command, ' ');
  return parseCommand(command, tokens);
}

/**
 * 유효한 user와 host인지 확인하는 함수
 * options의 [ '!' <user> ] [ '@' <host> ] 형식에 해당한다.
 * @param nick_and_host 확인할 user와 host가 포함된 options 부분
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 user와 host면 SUCCESS, 아니면 FAIL
 */
int parseUserAndHost(std::string user_and_host, std::vector<t_token> &tokens) {
  size_t pos = user_and_host.find('!');
  if (pos != std::string::npos) { // user가 있는 경우
    std::string user = user_and_host.substr(0, pos);
    if (user_and_host[pos + 1] == '\0' || user_and_host[pos + 1] == '@' || user.empty())
      return FAIL;
    tokens.push_back((t_token) {USER, user});
    pos = user_and_host.find('@');
    if (pos != std::string::npos) { // host가 있는 경우
      std::string host = user_and_host.substr(pos + 1);
      if (user_and_host[pos + 1] == '\0' || host.empty())
        return FAIL;
      tokens.push_back((t_token) {HOST, host});
    }
  }
  return SUCCESS;
}

/**
 * 유효한 명령어 형식인지 확인하는 함수
 * <command>  ::= <letter> { <letter> } | <number> <number> <number>
 * @param command_part 확인할 명령어 부분
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 command면 SUCCESS, 아니면 FAIL
 */
int parseCommand(std::string command_part, std::vector<t_token> &tokens) {
  if (command_part.empty())
    return FAIL;
  std::string command = command_part.substr(0, command_part.find(' '));
  if (std::isdigit(command[0])) {
    if (command.length() != 3)
      return FAIL;
    for (int i = 0; i < 3; i++) {
      if (!std::isdigit(command[i]))
        return FAIL;
    }
  } else {
    for (int i = 0; i < command.length(); i++) {
      if (!std::isalpha(command[i]))
        return FAIL;
    }
    if (!isExecutableCommand(command))
      return FAIL;
  }
  tokens.push_back((t_token) {COMMAND, command});
  return parseParams(command_part, tokens);
}

/**
 * 실행할 수 있는 명령어인지 확인하는 함수. KICK, INVITE, TOPIC, MODE만 실행 가능하다.
 * @param command_part 확인할 명령어 부분. 파라미터 부분을 제외한 명령어 부분이다.
 * @return 실행할 수 있는 명령어면 SUCCESS, 아니면 FAIL
 */
bool isExecutableCommand(std::string command_part) {
  if (command_part == "KICK" || command_part == "INVITE"
      || command_part == "TOPIC" || command_part == "MODE")
    return true;
  return false;
}

/**
 * 유효한 파라미터인지 확인하는 함수.
 * <params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
 * @param command_part 확인할 명령어 부분. 명령어 부분과 파라미터 부분이 같이 들어온다.
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 파라미터면 SUCCESS, 아니면 FAIL
 */
int parseParams(std::string command_part, std::vector<t_token> &tokens) {
  skipChar(command_part, ' ');
  if (command_part.empty())
    return FAIL;
  if (command_part[0] == ':') {
    return parseTrailing(command_part, tokens);
  } else {
    return parseMiddle(command_part, tokens);
  }
}

/**
 * 유효한 trailing인지 확인하는 함수.
 * <trailing> ::= <Any, possibly *empty*, sequence of octets not including
    NUL or CR or LF>
 * @param params 확인할 trailing. ':'로 시작한다.
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 trailing이면 SUCCESS, 아니면 FAIL
 */
int parseTrailing(std::string params, std::vector<t_token> &tokens) {
  for (int i = 1; i < params.length(); i++) {
    if (params[i] == '\0' || params[i] == '\r' || params[i] == '\n')
      return FAIL;
  }
  tokens.push_back((t_token) {PARAMS, params.substr(1)});
  return SUCCESS;
}

/**
 * 유효한 middle인지 확인하는 함수.
 * <middle>   ::= <Any *non-empty* sequence of octets not including SPACE
    or NUL or CR or LF, the first of which may not be ':'>
 * @param params 확인할 middle. ':'로 시작하지 않는다.
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 middle이면 SUCCESS, 아니면 FAIL
 */
int parseMiddle(std::string params, std::vector<t_token> &tokens) {
  if (params.empty() || params[0] == ':')
    return FAIL;
  for (int i = 0; i < params.length(); i++) {
    if (params[i] == ' ')
      return parseParams(params.substr(i), tokens);
    if (params[i] == '\0' || params[i] == '\r' || params[i] == '\n' || params[i] == ' ')
      return FAIL;
  }
  tokens.push_back((t_token) {PARAMS, params});
  return SUCCESS;
}

/**
 * 파싱된 토큰들을 출력하는 함수
 * @param tokens 파싱된 토큰들이 저장된 벡터
 */
void printTokens(std::vector<t_token> tokens) {
  std::cout << F_YELLOW << "[DEBUG] Tokens: " << FB_DEFAULT;
  for (int i = 0; i < tokens.size(); i++) {
    std::cout << tokens[i].value << " ";
  }
  std::cout << std::endl;
}