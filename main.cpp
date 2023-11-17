/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:39:21 by yena              #+#    #+#             */
/*   Updated: 2023/11/18 04:27:52 by heshin           ###   ########.fr       */
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
#include <sstream>

using std::cout;
using std::vector;
using std::string;

void add_user(const string& nick, const string& password, const string& ip_addr, RequestHandler& handler);

int main(int argc, char *argv[]) {

	RequestHandler handler;
	Reflector::shared().add(UserData::get_storage());

	add_user("heshin", "secret", "127.0.0.2", handler);
	add_user("heshin", "none", "127.0.0.3", handler);
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

void add_user(const string& nick, const string& password, const string& ip_addr, RequestHandler& handler) {

	static int socket = 0;
	Connection con;
	con.address = ip_addr;
	con.is_alive = true;
	con.socket_fd = socket++;
	con.port = 8080;

	try {
		vector<string> vec;
		vec.push_back("PASS");
		vec.push_back(":password");
		vec.push_back("is");
		if (!password.empty())
			vec.push_back(password);
		handler.get_request(vec, con);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return ;
	}

	try {
		vector<string> vec;
		vec.push_back("NICK");
		vec.push_back(nick);
		handler.get_request(vec, con);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return ;
	}

	try {
		vector<string> vec;
		std::stringstream ss;
		ss << " no." << socket;
		string name = ss.str();
		vec.push_back("USER");
		vec.push_back("username" + name);
		vec.push_back("hostname" + name);
		vec.push_back("servername" + name);
		vec.push_back("realname" + name);
		handler.get_request(vec, con);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return;
	}
}
