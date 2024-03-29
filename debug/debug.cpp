/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:55:59 by yena              #+#    #+#             */
/*   Updated: 2023/11/09 20:08:52 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/debug.hpp"
#include <ctime>

/**
 * 디버그 모드일 때 포트 번호를 입력받는 함수
 * @return 입력받은 포트 번호
 */
std::string getPortInDebugMode() {
  std::string port;

  std::cout << F_GREEN << "DEBUG MODE" << FB_DEFAULT << std::endl;
  std::cout << "Enter port number: ";
  std::cin >> port;
  return port;
}

std::string get_time_stamp() {

	time_t t;
	time(&t);
	char* time_str = ctime(&t);
	if (time_str != NULL) {
		std::string result = time_str;
		size_t newline = result.find('\n');
		if (newline != std::string::npos) {
			result.erase(newline);
		}
		return result;
	}
	else
	 	return "";
}
