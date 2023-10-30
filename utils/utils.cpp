/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:40:08 by yena              #+#    #+#             */
/*   Updated: 2023/10/30 22:14:22 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"

/**
 * 에러 메시지를 출력하는 함수
 * @param message 출력할 에러 메시지
 */
void printError(std::string message) {
  std::cout << F_RED << "Error: " << message << FB_DEFAULT << std::endl;
}

/**
 * 문자열이 숫자로만 이루어져있는지 확인하는 함수
 * @param str 확인할 문자열
 * @return 숫자로만 이루어져있으면 true, 아니면 false
 */
bool isNumber(const char *str) {
  int i = 0;

  while (str[i]) {
    if (!std::isdigit(str[i]))
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
bool isValidPort(const char *port) {
  if (!isNumber(port))
    return false;

  int i_port = std::atoi(port);
  if (i_port < 0 || i_port > 65535)
    return false;
  return true;
}
