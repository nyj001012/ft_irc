/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_server.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:23:09 by yena              #+#    #+#             */
/*   Updated: 2023/11/01 14:23:19 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server() {
  _max_client_number = MAX_CLIENT_NUM;
  _server_socket = 0;
  _fd_max = 0;
  std::memset(&_server_addr, 0, sizeof(_server_addr));
  std::memset(&_client_fds, 0, sizeof(_client_fds));
}

Server::Server(const Server &server) {
  if (this != &server) {
    _max_client_number = server._max_client_number;
    _server_socket = server._server_socket;
    _client_fds = server._client_fds;
    _fd_max = server._fd_max;
    _server_addr = server._server_addr;
    _is_debug = server._is_debug;
  }
}

Server &Server::operator=(const Server &server) {
  if (this != &server) {
    _max_client_number = server._max_client_number;
    _server_socket = server._server_socket;
    _client_fds = server._client_fds;
    _fd_max = server._fd_max;
    _server_addr = server._server_addr;
    _is_debug = server._is_debug;
  }
  return *this;
}

Server::~Server() {
  this->closeServer();
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

int Server::getFdMax() const {
  return this->_fd_max;
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

void Server::setFdMax(int fd_max) {
  this->_fd_max = fd_max;
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
  this->_server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (this->_server_socket == -1)
    throw std::runtime_error("Error: socket() failed");
  char *port_backup = new char[std::strlen(port) + 1];
  std::strcpy(port_backup, port);
  std::memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(std::atoi(port_backup));

  if (bind(this->_server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
    this->closeServer();
    throw std::runtime_error("Error: bind() failed");
  }
  if (this->_is_debug)
    std::cout << F_YELLOW << "[DEBUG] Server initialized: " << FB_DEFAULT << std::endl;
  delete[] port_backup;
  if (listen(this->_server_socket, MAX_CLIENT_NUM) == -1) {
    this->closeServer();
    throw std::runtime_error("Error: listen() failed");
  }
  this->_server_addr = serv_addr;
}

/**
 * 클라이언트를 초기화한다. 클라이언트 소켓을 생성하고, 클라이언트의 주소를 받아온다.
 */
void Server::initializeClient() {
  struct sockaddr_in addr_client;
  socklen_t addr_client_len = sizeof(addr_client_len);

  FD_ZERO(&this->_client_fds);
  FD_SET(this->_server_socket, &this->_client_fds);
  this->setFdMax(this->_server_socket);
}

/**
 * 서버를 실행한다. 클라이언트로부터 명령어를 받아서 처리한다.
 */
void Server::runServer() {
  struct timeval tv = {TIMEOUT_SEC, 0};

  while (true) {
    if (this->_is_debug) {
      std::cout << F_YELLOW << "[DEBUG] Server running..." << FB_DEFAULT << std::endl;
      std::cout << *this << std::endl;
    }
    int ready_descriptors = select(this->_fd_max + 1, &this->_client_fds, nullptr, nullptr, &tv);
    if (ready_descriptors == -1)
      throw std::runtime_error("Error: select() failed");
    if (ready_descriptors == 0)
      continue;
    for (int i = 0; i <= this->_fd_max; i++) {
      if (FD_ISSET(i, &this->_client_fds)) {
        if (i == this->_server_socket)
          this->acceptClient();
        else
          this->receiveMessage(i);
      }
    }
  }
}

void Server::acceptClient() {
  socklen_t client_addr_len = sizeof(struct sockaddr_in);
  struct sockaddr client_addr;
  int client_socket = accept(this->_server_socket, &client_addr, &client_addr_len);
  if (client_socket == -1)
    throw std::runtime_error("Error: accept() failed");
  FD_SET(client_socket, &this->_client_fds);
  if (client_socket > this->_fd_max)
    this->_fd_max = client_socket;
  if (this->_is_debug)
    std::cout << F_YELLOW << "[DEBUG] New client connected: " << client_socket << FB_DEFAULT << std::endl;
}

void Server::receiveMessage(int client_socket) {
  char buffer[BUFFER_SIZE];

  std::memset(buffer, 0, BUFFER_SIZE);
  ssize_t read_size = read(client_socket, buffer, BUFFER_SIZE);
  if (read_size == -1)
    throw std::runtime_error("Error: read() failed");
  if (read_size == 0) {
    this->closeClient(client_socket);
    return;
  }
  if (this->_is_debug)
    std::cout << F_YELLOW << "[DEBUG] Message received: " << buffer << FB_DEFAULT << std::endl;
  this->sendMessage(client_socket, buffer);
}

void Server::closeClient(int client_socket) {
  close(client_socket);
  FD_CLR(client_socket, &this->_client_fds);
  if (this->_is_debug)
    std::cout << F_YELLOW << "[DEBUG] Client closed: " << client_socket << FB_DEFAULT << std::endl;
}

void Server::sendMessage(int client_socket, char *message) {
  ssize_t write_size = write(client_socket, message, std::strlen(message));
  if (write_size == -1)
    throw std::runtime_error("Error: write() failed");
}

void Server::closeServer() {
  close(this->_server_socket);
}

std::ostream &operator<<(std::ostream &os, const fd_set &client_fds) {
  os << "[ ";
  for (int i = 0; i < FD_SETSIZE; i++) {
    if (FD_ISSET(i, &client_fds)) {
      os << i << std::endl;
      if (i != FD_SETSIZE - 1)
        os << ", ";
    }
  }
  os << " ]";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
  struct sockaddr_in server_addr = server.getServerAddr();

  os << F_YELLOW << "[DEBUG] Server: " << FB_DEFAULT << std::endl;
  os << "=> Server socket    : " << server.getServerSocket() << std::endl;
  os << "=> Server address   : " << inet_ntoa(server_addr.sin_addr) << std::endl;
  os << "=> Server port      : " << ntohs(server_addr.sin_port) << std::endl;
  os << "=> Max Client Number: " << server.getMaxClientNumber() << std::endl;
  os << "=> Client fds       : " << server.getClientFds() << std::endl;
  return os;
}
