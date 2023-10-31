/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_server.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:23:09 by yena              #+#    #+#             */
/*   Updated: 2023/10/31 16:30:04 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"

/**
 * 서버를 초기화한다. 서버 소켓을 생성하고, 주소를 바인딩한다.
 * memset() 함수를 이용해 serv_addr 구조체를 0으로 초기화하니 port가 0으로 초기화되어 port를 따로 복사해두었다.
 * @param port 서버의 포트
 */
void initializeServer(const char *port) {
  struct sockaddr_in serv_addr;
  server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

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
  if (is_debug)
    printServerInfo(serv_addr);
  delete[] port_backup;
  if (listen(server_socket, MAX_CLIENT_NUM) == -1) {
    close(server_socket);
    throw std::runtime_error("Error: listen() failed");
  }
}

/**
 * 서버의 정보를 출력한다.
 * @param server_socket 서버 소켓 fd값
 * @param serv_addr
 */
void printServerInfo(struct sockaddr_in serv_addr) {
  std::cout << F_CYAN << "[INFO] Server: " << FB_DEFAULT << std::endl;
  std::cout << "=> Server socket: " << server_socket << std::endl;
  std::cout << "=> Server address: " << inet_ntoa(serv_addr.sin_addr) << std::endl;
  std::cout << "=> Server port: " << ntohs(serv_addr.sin_port) << std::endl;
}

/**
 * 클라이언트를 초기화한다. 클라이언트 소켓을 생성하고, 클라이언트의 주소를 받아온다.
 */
void initializeClient() {
  struct sockaddr_in addr_client;
  socklen_t addr_client_len = sizeof(addr_client_len);

  std::memset(&addr_client, 0, sizeof(addr_client));
  client_socket = accept(server_socket, (struct sockaddr *) &addr_client, &addr_client_len);
  if (client_socket == -1) {
    close(server_socket);
    throw std::runtime_error("Error: accept() failed");
  }
}

/**
 * 서버를 실행한다. 클라이언트로부터 명령어를 받아서 처리한다.
 */
void runServer() {
  char buffer[BUFFER_SIZE];

  while (true) {
    std::memset(buffer, 0, BUFFER_SIZE);
    int read_size = read(client_socket, buffer, sizeof(buffer) - 1);
    if (read_size == -1) {
      std::cerr << "read() error" << std::endl;
      break;
    }
    buffer[std::strlen(buffer)] = '\n';
    buffer[std::strlen(buffer) - 1] = '\r';
    // TODO => 명령어 파싱
    // TODO => 명령어 실행하는 함수에 파싱된 명령어 전달
  }
  close(server_socket);
}
