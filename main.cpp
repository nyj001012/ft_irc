/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:39:21 by yena              #+#    #+#             */
/*   Updated: 2023/11/01 13:38:01 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/utils.hpp"
#include "include/debug.hpp"
#include "include/Server.hpp"

int main(int argc, char *argv[]) {
  const char *port;
  Server server;

  if (argc != 2) {
    printError("Invalid arguments: Usage: ./server <port>");
    return 1;
  }
  port = argv[1];
  if (!std::strcmp(argv[1], "DEBUG")) {
    port = getPortInDebugMode();
    server.setIsDebug(true);
  }
  if (!isValidPort(port)) {
    printError("Error: Invalid port number: " + static_cast<std::string>(port));
    return 1;
  }
  try {
    server.initializeServer(port);
    server.initializeClientFds();
    server.runServer();
  } catch (std::exception &e) {
    printError(e.what());
    return 1;
  }
  return 0;
}
