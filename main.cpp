/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:39:21 by yena              #+#    #+#             */
/*   Updated: 2023/11/16 01:18:30 by heshin           ###   ########.fr       */
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
#include "handler/RequestHandler.hpp"
#include <cstring>
#include <exception>
#include <iterator>
#include <iostream>

using std::cout;
using std::vector;
using std::string;

int main(int argc, char *argv[]) {

	Connection con;
	con.address = "127.0.0.1";
	con.is_alive = true;
	con.socket_fd = 11;
	con.port = 8080;
	RequestHandler handler;
	Reflector::shared().add(UserData::get_storage());

	try {
		vector<string> vec;
		vec.push_back("PASS");
		vec.push_back("secret");
		handler.get_request(vec, con);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	try {
		vector<string> vec;
		vec.push_back("NICK");
		vec.push_back("heshin");
		handler.get_request(vec, con);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	try {
		vector<string> vec;
		vec.push_back("USER");
		vec.push_back("username test");
		vec.push_back("hostname test");
		vec.push_back("servername test");
		vec.push_back("realname test");
		handler.get_request(vec, con);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	Reflector::shared().update();
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
