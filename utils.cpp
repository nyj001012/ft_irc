/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:40:08 by yena              #+#    #+#             */
/*   Updated: 2023/10/30 21:45:08 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool isNumber(char *str) {
  int i = 0;

  while (str[i]) {
    if (!std::isdigit(str[i]))
      return false;
    i++;
  }
  return true;
}

bool isValidPort(char *port) {
  if (!isNumber(port))
    return false;

  int i_port = std::atoi(port);
  if (i_port < 0 || i_port > 65535)
    return false;
  return true;
}
