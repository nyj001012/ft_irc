/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:55:59 by yena              #+#    #+#             */
/*   Updated: 2023/10/30 22:13:06 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/debug.hpp"

/**
 * 디버그 모드일 때 포트 번호를 입력받는 함수
 * @return 입력받은 포트 번호
 */
const char *getPortInDebugMode() {
  std::string port;

  std::cout << F_GREEN << "DEBUG MODE" << FB_DEFAULT << std::endl;
  std::cout << "Enter port number: ";
  std::cin >> port;
  return port.c_str();
}
