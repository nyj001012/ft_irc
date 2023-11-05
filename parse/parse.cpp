/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:43:05 by yena              #+#    #+#             */
/*   Updated: 2023/11/05 16:16:40 by yena             ###   ########.fr       */
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
 * @return 유효한 명령어면 true, 아니면 false
 */
bool isValidMessageFormat(std::string command, bool is_debug) {
  bool result;
  if (command.empty() || command[command.length() - 1] != '\n')
    result = false;
  else if (command[0] == ':')
    result = isValidCommandWithOptions(command);
  else
    result = isValidCommand(command);
  if (is_debug)
    std::cout << F_YELLOW<< "[DEBUG] isValidMessageFormat \""
              << command << "\": " << result << FB_DEFAULT << std::endl;
  return result;
}

/**
 * 유효한 options가 있는 명령어인지 확인하는 함수
 * options는 <servername> | <nick> [ '!' <user> ] [ '@' <host> ] 형식이어야 한다.
 * @param command 확인할 명령어, command[0] == ':'
 * @return 유효한 options면 true, 아니면 false
 */
bool isValidCommandWithOptions(std::string command) {
  size_t pos = command.find(' ');
  if (pos == std::string::npos)
    return false;
  std::string options = command.substr(1, pos - 1);
  if (options.empty())
    return false;
  pos = options.find('!');
  if (pos != std::string::npos) { // user가 있는 경우
    std::string nick = options.substr(0, pos);
    if (options[pos + 1] == '\0' || options[pos + 1] == '@' || nick.empty())
      return false;
    pos = options.find('@');
    if (pos != std::string::npos) { // host가 있는 경우
      std::string user = options.substr(pos + 1);
      if (options[pos + 1] == '\0' || user.empty())
        return false;
    }
  }
  command = command.substr(command.find(' ') + 1);
  return isValidCommand(command);
}

/**
 * 유효한 명령어 형식인지 확인하는 함수
 * <command>  ::= <letter> { <letter> } | <number> <number> <number>
 * @param command_part 확인할 명령어 부분
 * @return 유효한 command면 true, 아니면 false
 */
bool isValidCommand(std::string command_part) {
  if (command_part.empty())
    return false;
  if (std::isdigit(command_part[0])) {
    if (command_part.length() != 3)
      return false;
    for (int i = 0; i < 3; i++) {
      if (!std::isdigit(command_part[i]))
        return false;
    }
  } else {
    for (int i = 0; i < command_part.length(); i++) {
      if (!std::isalpha(command_part[i]))
        return false;
    }
    if (!isExecutableCommand(command_part))
      return false;
  }
  return isValidParams(command_part);
}

/**
 * 실행할 수 있는 명령어인지 확인하는 함수. KICK, INVITE, TOPIC, MODE만 실행 가능하다.
 * @param command_part 확인할 명령어 부분. 명령어 부분과 파라미터 부분이 같이 들어온다.
 * @return 실행할 수 있는 명령어면 true, 아니면 false
 */
bool isExecutableCommand(std::string command_part) {
  size_t pos = command_part.find(' ');
  std::string command = command_part.substr(0, pos);
  if (command == "KICK" || command == "INVITE"
      || command == "TOPIC" || command == "MODE")
    return true;
  return false;
}

/**
 * 유효한 파라미터인지 확인하는 함수.
 * <params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
 * @param command_part 확인할 명령어 부분. 명령어 부분과 파라미터 부분이 같이 들어온다.
 * @return 유효한 파라미터면 true, 아니면 false
 */
bool isValidParams(std::string command_part) {
  if (command_part.empty())
    return true;
  size_t pos = command_part.find(' ');
  if (pos == std::string::npos)
    return true;
  std::string params = command_part.substr(pos + 1);
  if (params[0] == ':') {
    return isValidTrailing(params);
  } else {
    return isValidMiddle(params);
  }
}

/**
 * 유효한 trailing인지 확인하는 함수.
 * <trailing> ::= <Any, possibly *empty*, sequence of octets not including
    NUL or CR or LF>
 * @param params 확인할 trailing. ':'로 시작한다.
 * @return 유효한 trailing이면 true, 아니면 false
 */
bool isValidTrailing(std::string params) {
  if (params.empty())
    return false;
  for (int i = 1; i < params.length(); i++) {
    if (params[i] == '\0' || params[i] == '\r' || params[i] == '\n')
      return false;
  }
  return true;
}

/**
 * 유효한 middle인지 확인하는 함수.
 * <middle>   ::= <Any *non-empty* sequence of octets not including SPACE
    or NUL or CR or LF, the first of which may not be ':'>
 * @param params 확인할 middle. ':'로 시작하지 않는다.
 * @return 유효한 middle이면 true, 아니면 false
 */
bool isValidMiddle(std::string params) {
  if (params.empty())
    return false;
  for (int i = 0; i < params.length(); i++) {
    if (params[i] == '\0' || params[i] == '\r' || params[i] == '\n' || params[i] == ' ')
      return false;
  }
  return true;
}