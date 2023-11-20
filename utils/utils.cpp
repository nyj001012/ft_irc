/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:40:08 by yena              #+#    #+#             */
/*   Updated: 2023/11/19 15:13:32 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"
#include <cstdlib>

/**
 * 에러 메시지를 출력하는 함수
 * @param message 출력할 에러 메시지
 */
void printError(std::string message)
{
	std::cout << F_RED << "Error: " << message << FB_DEFAULT << std::endl;
}

/**
 * 문자열이 숫자로만 이루어져있는지 확인하는 함수
 * @param string 확인할 문자열
 * @return 숫자로만 이루어져있으면 true, 아니면 false
 */
bool isNumber(const char* string)
{
	int i = 0;

	while (string[i])
	{
		if (!std::isdigit(string[i]))
			return false;
		i++;
	}
	return true;
}

/**
 * 유효한 포트인지 확인하는 함수. 포트는 0 ~ 65535 사이의 숫자여야 한다.
 * @param port 확인할 포트 문자열
 * @return 유효한 포트면 true, 아니면 false
 */
bool isValidPort(const char* port)
{
	if (!isNumber(port))
		return false;

	int i_port = atoi(port);
	if (i_port < 0 || i_port > 65535)
		return false;
	return true;
}

/**
 * 문자열에서 앞에서부터 c를 찾아 스킵하는 함수
 * @param string 스킵될 문자열. 문자열에 처음으로 나오는 c부터 c가 아닌 문자가 나올 때까지 스킵된다.
 * @param c 스킵할 문자
 */
void skipChar(std::string& string, char c)
{
	if (string.empty())
		return;
	size_t pos = string.find_first_of(c);
	if (pos != std::string::npos)
	{
		while (string[pos] == ' ')
			pos++;
		string = string.substr(pos);
	}
}
