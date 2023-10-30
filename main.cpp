/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:39:21 by yena              #+#    #+#             */
/*   Updated: 2023/10/30 22:46:27 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/utils.hpp"
#include "include/debug.hpp"
#include "include/server.hpp"

bool is_debug = false;
int server_socket = 0;

int main(int argc, char *argv[]) {
  const char *port;

  if (argc != 2) {
    printError("Invalid arguments: Usage: ./server <port>");
    return 1;
  }
  port = argv[1];
  if (!std::strcmp(argv[1], "DEBUG")) {
    port = getPortInDebugMode();
    is_debug = true;
  }
  if (!isValidPort(port)) {
    printError("Error: Invalid port number: " + static_cast<std::string>(port));
    return 1;
  }
  initializeServer(port);
//  runServer();
  return 0;
}
