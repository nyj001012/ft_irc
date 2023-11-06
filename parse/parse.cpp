/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:43:05 by yena              #+#    #+#             */
/*   Updated: 2023/11/06 19:02:41 by yena             ###   ########.fr       */
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
  // TODO => 여러 스페이스를 스킵하는 함수 적용
  bool result;
  if (command.empty() || command[command.length() - 1] != '\n')
    result = false;
  command.erase(command.length() - 1);
  if (command[0] == ':')
    result = isValidCommandWithOptions(command);
  else
    result = isValidCommand(command);
  if (is_debug)
    std::cout << F_YELLOW << "[DEBUG] isValidMessageFormat \""
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
  if (!isValidUserAndHost(options)) {
    return false;
  }
  skipChar(command, ' ');
  return isValidCommand(command);
}

/**
 * 유효한 user와 host인지 확인하는 함수
 * options의 [ '!' <user> ] [ '@' <host> ] 형식에 해당한다.
 * @param nick_and_host 확인할 user와 host가 포함된 options 부분
 * @return 유효한 user와 host면 true, 아니면 false
 */
bool isValidUserAndHost(std::string nick_and_host) {
  size_t pos = nick_and_host.find('!');
  if (pos != std::string::npos) { // user가 있는 경우
    std::string nick = nick_and_host.substr(0, pos);
    if (nick_and_host[pos + 1] == '\0' || nick_and_host[pos + 1] == '@' || nick.empty())
      return false;
    pos = nick_and_host.find('@');
    if (pos != std::string::npos) { // host가 있는 경우
      std::string user = nick_and_host.substr(pos + 1);
      if (nick_and_host[pos + 1] == '\0' || user.empty())
        return false;
    }
  }
  return true;
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
  std::string command = command_part.substr(0, command_part.find(' '));
  if (std::isdigit(command[0])) {
    if (command.length() != 3)
      return false;
    for (int i = 0; i < 3; i++) {
      if (!std::isdigit(command[i]))
        return false;
    }
  } else {
    for (int i = 0; i < command.length(); i++) {
      if (!std::isalpha(command[i]))
        return false;
    }
    if (!isExecutableCommand(command))
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
  command_part = command_part.substr(0, command_part.find(' '));
  if (command_part == "KICK" || command_part == "INVITE"
      || command_part == "TOPIC" || command_part == "MODE")
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
  skipChar(command_part, ' ');
  if (command_part.empty())
    return false;
  if (command_part[0] == ':') {
    return isValidTrailing(command_part);
  } else {
    return isValidMiddle(command_part);
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
  if (params.empty() || params[0] == ':')
    return false;
  for (int i = 0; i < params.length(); i++) {
    if (params[i] == '\0' || params[i] == '\r' || params[i] == '\n' || params[i] == ' ')
      return false;
  }
  return true;
}