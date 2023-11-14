/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:39:21 by yena              #+#    #+#             */
/*   Updated: 2023/11/15 03:57:32 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "task/Task.hpp"
#include "channel/Channel.hpp"
#include "data/ChannelData.hpp"
#include "include/utils.hpp"
#include "data/UserData.hpp"
#include "include/debug.hpp"
#include "debug/Reflector.hpp"
#include "server/Server.hpp"
#include "user/User.hpp"
#include <cstring>
#include <exception>
#include <iterator>
#include <iostream>

using std::cout;
using std::vector;
using std::string;

int main(int argc, char *argv[]) {

	vector<string> vec;
	vec.push_back(":hello");
	vec.push_back("PASS");
	vec.push_back("secret");
	try {
		Task* task = Task::create(vec); 
		cout << "prefix " << task->get_prefix() << std::endl;
		cout << task->_serialize(1) << std::endl;
		delete task;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
	 const char *port;
  Server server;

  if (argc != 2) {
    printError("Invalid arguments: Usage: ./server <port>");
    return 1;
  }
  port = argv[1];
  if (!std::strcmp(argv[1], "DEBUG")) {
    port = (getPortInDebugMode()).c_str();
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
