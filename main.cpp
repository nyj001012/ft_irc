/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:39:21 by yena              #+#    #+#             */
/*   Updated: 2023/11/25 21:52:38 by yena             ###   ########.fr       */
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
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iterator>
#include <iostream>
#include <sstream>

using std::cout;
using std::vector;
using std::string;

void add_user(const string& nick, const string& password, const string& ip_addr, RequestHandler& handler);

int main(int argc, char* argv[])
{
	RequestHandler handler;
	Reflector::shared().add(UserData::get_storage());
	Reflector::shared().add(ChannelData::get_storage());
	char* port;
	Server server;

	if (argc == 3 || (argc == 2 && !std::strcmp(argv[1], "DEBUG")))
	{
		port = argv[1];
		if (!std::strcmp(argv[1], "DEBUG"))
		{
			std::strcpy(port, getPortInDebugMode().c_str());
			server.setIsDebug(true);
			Reflector::shared().set_debug(true);
		}
		else {
			server.setIsDebug(false);
			Reflector::shared().set_debug(false);
		}
		if (!isValidPort(port))
		{
			printError("Error: Invalid port number: " + static_cast<std::string>(port));
			return 1;
		}
		server.initializeServer(port);
		while (true)
		{
			try
			{
				server.runServer();
			}
			catch (std::exception& e)
			{
				printError(e.what());
			}
		}
	}
	else
	{
		printError("Invalid arguments: Usage: ./ircserv <port> <password>");
		return 1;
	}
}
