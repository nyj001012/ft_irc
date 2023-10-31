/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_server.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:23:09 by yena              #+#    #+#             */
/*   Updated: 2023/10/31 16:18:13 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_SERVER_INITIALIZE_HPP_
#define FT_IRC_SERVER_INITIALIZE_HPP_

#include "common.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iomanip>
#include <unistd.h>

#define MAX_CLIENT_NUM 10

void initializeServer(const char *port);
void printServerInfo(struct sockaddr_in serv_addr);
void initializeClient();
void runServer();

#endif //FT_IRC_SERVER_INITIALIZE_HPP_
