/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:43:05 by yena              #+#    #+#             */
/*   Updated: 2023/11/27 14:31:58 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.hpp"

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
int parseMessageFormat(std::string command, bool is_debug, std::vector<t_token>& tokens)
{
	bool result;
	if (command.empty() || command[command.length() - 1] != '\n')
		result = FAIL;
	size_t crlf_pos = command.find("\r\n");
	std::string trimmed_command = command.substr(0, crlf_pos);
	if (trimmed_command[0] == ':')
		result = parseCommandWithOptions(trimmed_command, tokens);
	else
		result = parseCommand(trimmed_command, tokens);
	reorderTrailing(tokens);
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
int parseCommandWithOptions(std::string command, std::vector<t_token>& tokens)
{
	size_t pos = command.find(' ');
	if (pos == std::string::npos)
		return FAIL;
	std::string options = command.substr(1, pos - 1);
	if (options.empty())
		return FAIL;
	if (!parseUserAndHost(options, tokens))
	{
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
int parseUserAndHost(std::string user_and_host, std::vector<t_token>& tokens)
{
	size_t user_pos = user_and_host.find('!');
	size_t host_pos = user_and_host.find('@');
	if (user_pos != std::string::npos)
	{ // user가 있는 경우
		if (user_pos == 0 || user_pos == user_and_host.length() - 1)
			return FAIL;
		std::string user = user_and_host.substr(user_pos + 1);
		if (host_pos != std::string::npos) // user와 host 모두 있는 경우
			user = user.substr(0, host_pos - user_pos - 1);
		tokens.push_back((t_token){ NICK, user_and_host.substr(0, user_pos) });
		tokens.push_back((t_token){ USER, user });
	}
	if (host_pos != std::string::npos)
	{ // host가 있는 경우
		if (host_pos == 0 || host_pos == user_and_host.length() - 1)
			return FAIL;
		if (user_pos == std::string::npos) // user 없이 host만 있는 경우
			tokens.push_back((t_token){ NICK, user_and_host.substr(0, host_pos) });
		tokens.push_back((t_token){ HOST, user_and_host.substr(host_pos + 1) });
	}
	if (user_pos == std::string::npos && host_pos == std::string::npos) // user, host가 없는 경우
		tokens.insert(tokens.begin(), (t_token){ NICK, user_and_host });
	return SUCCESS;
}

/**
 * 유효한 명령어 형식인지 확인하는 함수
 * <command>  ::= <letter> { <letter> } | <number> <number> <number>
 * @param command_part 확인할 명령어 부분
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 command면 SUCCESS, 아니면 FAIL
 */
int parseCommand(std::string command_part, std::vector<t_token>& tokens)
{
	if (command_part.empty())
		return FAIL;
	std::string command = command_part.substr(0, command_part.find(' '));
	if (std::isdigit(command[0]))
	{
		if (command.length() != 3)
			return FAIL;
		for (size_t i = 0; i < 3; i++)
		{
			if (!std::isdigit(command[i]))
				return FAIL;
		}
	}
	else
	{
		for (size_t i = 0; i < command.length(); i++)
		{
			if (!std::isalpha(command[i]))
				return FAIL;
		}
	}
	tokens.push_back((t_token){ COMMAND, command });
	return parseParams(command_part, tokens);
}

/**
 * 유효한 파라미터인지 확인하는 함수.
 * <params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
 * @param command_part 확인할 명령어 부분. 명령어 부분과 파라미터 부분이 같이 들어온다.
 * @param tokens 파싱된 토큰들이 저장될 벡터
 * @return 유효한 파라미터면 SUCCESS, 아니면 FAIL
 */
int parseParams(std::string command_part, std::vector<t_token>& tokens)
{
	if (command_part.find(' ') == std::string::npos)
		return SUCCESS;
	skipChar(command_part, ' ');
	if (command_part.empty())
		return FAIL;
	if (command_part[0] == ':')
	{
		return parseTrailing(command_part, tokens);
	}
	else
	{
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
int parseTrailing(std::string params, std::vector<t_token>& tokens)
{
	for (size_t i = 1; i < params.length(); i++)
	{
		if (params[i] == '\0' || params[i] == '\r' || params[i] == '\n')
			return FAIL;
	}
	tokens.push_back((t_token){ TRAILING, params.substr(1) });
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
int parseMiddle(std::string params, std::vector<t_token>& tokens)
{
	if (params.empty() || params[0] == ':')
		return FAIL;
	for (size_t i = 0; i < params.length(); i++)
	{
		if (params[i] == ' ')
		{
			std::string middle = params.substr(0, i);
			tokens.push_back((t_token){ MIDDLE, middle });
			if (parseParams(params.substr(i), tokens) == FAIL)
				return FAIL;
			return SUCCESS;
		}
		if (params[i] == '\0' || params[i] == '\r' || params[i] == '\n' || params[i] == ' ')
			return FAIL;
	}
	tokens.push_back((t_token){ MIDDLE, params });
	return SUCCESS;
}

/**
 * trailing이 middle보다 앞에 오도록 순서를 바꾸는 함수
 * @param tokens 파싱된 토큰들이 저장된 벡터
 */
void reorderTrailing(std::vector<t_token>& tokens)
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].type == TRAILING)
		{
			t_token trailing = tokens[i];
			tokens.erase(tokens.begin() + i);
			tokens.push_back(trailing);
			break;
		}
	}
}

/**
 * 파싱된 토큰들을 출력하는 함수
 * @param tokens 파싱된 토큰들이 저장된 벡터
 */
void printTokens(std::vector<t_token> tokens)
{
	std::cout << F_YELLOW << "[DEBUG] Tokens: " << FB_DEFAULT << std::endl;
	size_t type_length = getLongestTokenType(tokens);
	size_t value_length = getLongestTokenValue(tokens);

	std::cout << F_BLACK_W_WHITE << "| " << std::setw(type_length) << "TYPE" << " | ";
	std::cout << std::setw(value_length) << "VALUE" << " |" << FB_DEFAULT << std::endl;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		std::cout << "| " << std::setw(type_length) << tokens[i].type << " | ";
		std::cout << std::setw(value_length) << tokens[i].value << " |" << std::endl;
	}
}

/**
 * 토큰들 중 가장 긴 type의 길이를 구하는 함수
 * @param tokens 토큰들이 저장된 벡터
 * @return 가장 긴 type의 길이
 */
size_t getLongestTokenType(std::vector<t_token> tokens)
{
	size_t type_length = 4;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].type.length() > type_length)
			type_length = tokens[i].type.length();
	}
	return type_length;
}

/**
 * 토큰들 중 가장 긴 value의 길이를 구하는 함수
 * @param tokens 토큰들이 저장된 벡터
 * @return 가장 긴 value의 길이
 */
size_t getLongestTokenValue(std::vector<t_token> tokens)
{
	size_t value_length = 5;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].value.length() > value_length)
			value_length = tokens[i].value.length();
	}
	return value_length;
}

/**
 * 토큰들의 value만을 저장하여 벡터로 반환한다.
 * @param tokens
 * @return 토큰들의 value만을 저장한 벡터
 */
std::vector<std::string> getTokensValue(std::vector<t_token> tokens)
{
	std::vector<std::string> tokens_value;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		tokens_value.push_back(tokens[i].value);
	}
	return tokens_value;
}
