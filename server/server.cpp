/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_server.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:23:09 by yena              #+#    #+#             */
/*   Updated: 2023/10/30 22:53:18 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"

void initializeServer(const char *port) {
  struct sockaddr_in serv_addr;
  server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  std::memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(port));

  bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (is_debug)
    printServerInfo(server_socket, serv_addr);
}

void printServerInfo(int server_socket, struct sockaddr_in serv_addr) {
  std::cout << F_CYAN << "[INFO] Server: " << FB_DEFAULT << std::endl;
  std::cout << "=> Server socket: " << server_socket << std::endl;
  std::cout << "=> Server address: " << inet_ntoa(serv_addr.sin_addr) << std::endl;
  std::cout << "=> Server port: " << ntohs(serv_addr.sin_port) << std::endl;
}
