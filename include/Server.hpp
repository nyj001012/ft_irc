/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_server.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:23:09 by yena              #+#    #+#             */
/*   Updated: 2023/11/01 14:17:59 by yena             ###   ########.fr       */
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
#define BUFFER_SIZE 512
#define TIMEOUT_SEC 5

class Server {
 public:
  Server();
  Server(const Server &server);
  Server &operator=(const Server &server);
  ~Server();
  int getMaxClientNumber() const;
  int getServerSocket() const;
  fd_set getClientFds() const;
  int getFdMax() const;
  struct sockaddr_in getServerAddr() const;
  bool getIsDebug() const;
  void setMaxClientNumber(int max_client_number);
  void setServerSocket(int server_socket);
  void setClientFds(fd_set client_fds);
  void setFdMax(int fd_max);
  void setServerAddr(struct sockaddr_in server_addr);
  void setIsDebug(bool is_debug);
  void initializeServer(const char *port);
  void initializeClient();
  void runServer();
  void acceptClient();
  void receiveMessage(int client_socket);
  void sendMessage(int client_socket, char *message);
  void closeClient(int client_socket);
  void closeServer();

 private:
  int _max_client_number;
  int _server_socket;
  fd_set _client_fds;
  int _fd_max;
  struct sockaddr_in _server_addr;
  bool _is_debug;
};

std::ostream &operator<<(std::ostream &os, const Server &server);
std::ostream &operator<<(std::ostream &os, const fd_set &client_fds);

#endif //FT_IRC_SERVER_INITIALIZE_HPP_
