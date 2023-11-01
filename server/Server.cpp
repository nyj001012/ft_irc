/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_server.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:23:09 by yena              #+#    #+#             */
/*   Updated: 2023/11/01 13:37:27 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server() {
  _max_client_number = MAX_CLIENT_NUM;
  _server_socket = 0;
  std::memset(&_server_addr, 0, sizeof(_server_addr));
  std::memset(&_client_fds, 0, sizeof(_client_fds));
}

Server::Server(int _max_client_number, int _server_socket, fd_set _client_fds) :
    _max_client_number(_max_client_number), _server_socket(_server_socket), _client_fds(_client_fds) {}

Server::Server(const Server &server) {
  if (this != &server) {
    _max_client_number = server._max_client_number;
    _server_socket = server._server_socket;
    _client_fds = server._client_fds;
  }
}

Server &Server::operator=(const Server &server) {
  if (this != &server) {
    _max_client_number = server._max_client_number;
    _server_socket = server._server_socket;
    _client_fds = server._client_fds;
  }
  return *this;
}

Server::~Server() {
  close(_server_socket);
}

int Server::getMaxClientNumber() const {
  return this->_max_client_number;
}

int Server::getServerSocket() const {
  return this->_server_socket;
}

fd_set Server::getClientFds() const {
  return this->_client_fds;
}

struct sockaddr_in Server::getServerAddr() const {
  return this->_server_addr;
}

bool Server::getIsDebug() const {
  return this->_is_debug;
}

void Server::setMaxClientNumber(int max_client_number) {
  this->_max_client_number = max_client_number;
}

void Server::setServerSocket(int server_socket) {
  this->_server_socket = server_socket;
}

void Server::setClientFds(fd_set client_fds) {
  this->_client_fds = client_fds;
}

void Server::setServerAddr(struct sockaddr_in server_addr) {
  this->_server_addr = server_addr;
}

void Server::setIsDebug(bool is_debug) {
  this->_is_debug = is_debug;
}

/**
 * 서버를 초기화한다. 서버 소켓을 생성하고, 주소를 바인딩한다.
 * memset() 함수를 이용해 serv_addr 구조체를 0으로 초기화하니 port가 0으로 초기화되어 port를 따로 복사해두었다.
 * @param port 서버의 포트
 */
void Server::initializeServer(const char *port) {
  struct sockaddr_in serv_addr;
  int server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (server_socket == -1)
    throw std::runtime_error("Error: socket() failed");
  char *port_backup = new char[std::strlen(port) + 1];
  std::strcpy(port_backup, port);
  std::memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(std::atoi(port_backup));

  if (bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
    close(server_socket);
    throw std::runtime_error("Error: bind() failed");
  }
  if (this->getIsDebug())
    printServerInfo(serv_addr);
  delete[] port_backup;
  if (listen(server_socket, MAX_CLIENT_NUM) == -1) {
    close(server_socket);
    throw std::runtime_error("Error: listen() failed");
  }
  this->setServerSocket(server_socket);
  this->setServerAddr(serv_addr);
}

/**
 * 서버의 정보를 출력한다.
 * @param _server_socket 서버 소켓 fd값
 * @param serv_addr
 */
void Server::printServerInfo(struct sockaddr_in serv_addr) {
}

/**
 * 클라이언트를 초기화한다. 클라이언트 소켓을 생성하고, 클라이언트의 주소를 받아온다.
 */
void Server::initializeClient() {
  struct sockaddr_in addr_client;
  socklen_t addr_client_len = sizeof(addr_client_len);

  std::memset(&addr_client, 0, sizeof(addr_client));
  client_socket = accept(_server_socket, (struct sockaddr *) &addr_client, &addr_client_len);
  if (client_socket == -1) {
    close(_server_socket);
    throw std::runtime_error("Error: accept() failed");
  }
}

/**
 * 서버를 실행한다. 클라이언트로부터 명령어를 받아서 처리한다.
 */
void Server::runServer() {
  char buffer[BUFFER_SIZE];

  while (true) {
    std::memset(buffer, 0, BUFFER_SIZE);
    int read_size = read(client_socket, buffer, sizeof(buffer) - 2);
    if (read_size == -1) {
      std::cerr << "read() error" << std::endl;
      break;
    }
    std::strcat(buffer, "\r\n");
    if (is_debug)
      std::cout << F_CYAN << "[ INFO ] " << FB_DEFAULT << "Client:\n" << buffer << std::endl;
    // TODO => 명령어 파싱
    // TODO => 명령어 실행하는 함수에 파싱된 명령어 전달
  }
  close(_server_socket);
}

std::ostream &operator<<(std::ostream &os, const fd_set &client_fds) {
  os << "[ ";
  for (int i = 0; i < FD_SETSIZE; i++) {
    if (FD_ISSET(i, &client_fds))
      os << i << std::endl;
    if (i != FD_SETSIZE - 1)
      os << ", ";
  }
  os << " ]";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
  struct sockaddr_in server_addr = server.getServerAddr();

  os << F_CYAN << "[INFO] Server: " << FB_DEFAULT << std::endl;
  os << "=> Server socket    : " << server.getServerSocket() << std::endl;
  os << "=> Server address   : " << inet_ntoa(server_addr.sin_addr) << std::endl;
  os << "=> Server port      : " << ntohs(server_addr.sin_port) << std::endl;
  os << "=> Max Client Number: " << server.getMaxClientNumber() << std::endl;
  os << "=> Client fds       : " << server.getClientFds() << std::endl;
  return os;
}
