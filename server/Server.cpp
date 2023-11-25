/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 20:28:24 by yena              #+#    #+#             */
/*   Updated: 2023/11/25 19:53:19 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../user/User.hpp"
#include <cstring>
#include <cstdlib>

Server::Server()
{
	_max_client_number = MAX_CLIENT_NUM;
	_server_socket = 0;
	_fd_max = 0;
	std::memset(&_server_addr, 0, sizeof(_server_addr));
	std::memset(&_read_fds, 0, sizeof(_read_fds));
	std::memset(&_write_fds, 0, sizeof(_write_fds));
	std::memset(&_read_fds_backup, 0, sizeof(_read_fds_backup));
	std::memset(&_write_fds_backup, 0, sizeof(_write_fds_backup));
}

Server::Server(const Server& server)
{
	if (this != &server)
	{
		_max_client_number = server._max_client_number;
		_server_socket = server._server_socket;
		_read_fds = server._read_fds;
		_write_fds = server._write_fds;
		_read_fds_backup = server._read_fds_backup;
		_write_fds_backup = server._write_fds_backup;
		_fd_max = server._fd_max;
		_server_addr = server._server_addr;
		_is_debug = server._is_debug;
	}
}

Server& Server::operator=(const Server& server)
{
	if (this != &server)
	{
		_max_client_number = server._max_client_number;
		_server_socket = server._server_socket;
		_read_fds = server._read_fds;
		_write_fds = server._write_fds;
		_read_fds_backup = server._read_fds_backup;
		_write_fds_backup = server._write_fds_backup;
		_fd_max = server._fd_max;
		_server_addr = server._server_addr;
		_is_debug = server._is_debug;
	}
	return *this;
}

Server::~Server()
{
	this->closeServer();
}

int Server::getMaxClientNumber() const
{
	return this->_max_client_number;
}

int Server::getServerSocket() const
{
	return this->_server_socket;
}

fd_set Server::getReadFds() const
{
	return this->_read_fds;
}

fd_set Server::getWriteFds() const
{
	return this->_write_fds;
}

fd_set Server::getReadFdsBackup() const
{
	return this->_read_fds_backup;
}

fd_set Server::getWriteFdsBackup() const
{
	return this->_write_fds_backup;
}

int Server::getFdMax() const
{
	return this->_fd_max;
}

struct sockaddr_in Server::getServerAddr() const
{
	return this->_server_addr;
}

bool Server::getIsDebug() const
{
	return this->_is_debug;
}

void Server::setMaxClientNumber(int max_client_number)
{
	this->_max_client_number = max_client_number;
}

void Server::setServerSocket(int server_socket)
{
	this->_server_socket = server_socket;
}

void Server::setReadFds(fd_set fds)
{
	this->_read_fds = fds;
}

void Server::setWriteFds(fd_set fds)
{
	this->_write_fds = fds;
}

void Server::setReadFdsBackup(fd_set fds)
{
	this->_read_fds_backup = fds;
}

void Server::setWriteFdsBackup(fd_set fds)
{
	this->_write_fds_backup = fds;
}

void Server::setFdMax(int fd_max)
{
	this->_fd_max = fd_max;
}

void Server::setServerAddr(struct sockaddr_in server_addr)
{
	this->_server_addr = server_addr;
}

void Server::setIsDebug(bool is_debug)
{
	this->_is_debug = is_debug;
}

/**
 * 서버를 초기화한다. 서버 소켓을 생성하고, 주소를 바인딩한다.
 * @param port 서버의 포트
 */
void Server::initializeServer(const char* port)
{
	struct sockaddr_in serv_addr;
	this->_server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->_server_socket == -1)
		throw std::runtime_error("Error: socket() failed");
	fcntl(this->_server_socket, F_SETFL, O_NONBLOCK);
	std::memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_UNSPEC;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	if (bind(this->_server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		this->closeServer();
		throw std::runtime_error("Error: bind() failed");
	}
	if (this->_is_debug)
		std::cout << F_YELLOW << "[DEBUG] Server initialized: " << FB_DEFAULT << std::endl;
	if (listen(this->_server_socket, MAX_CLIENT_NUM) == -1)
	{
		this->closeServer();
		throw std::runtime_error("Error: listen() failed");
	}
	this->_server_addr = serv_addr;
}

/**
 * 클라이언트에 할당할 fd_set을 초기화한다. 서버 소켓을 fd_set에 추가한다.
 */
void Server::initializeClientFds()
{
	FD_ZERO(&this->_read_fds);
	FD_ZERO(&this->_write_fds);
	FD_ZERO(&this->_read_fds_backup);
	FD_ZERO(&this->_write_fds_backup);
	FD_SET(this->_server_socket, &this->_read_fds);
	this->setFdMax(this->_server_socket);
}

/**
 * 서버를 실행한다.
 * select() 함수를 이용해 이벤트가 발생했는지 확인한다.
 * 이벤트가 발생했다면 acceptClient() 또는 receiveMessage() 함수를 호출한다.
 * 이벤트가 발생하지 않았다면 다시 select() 함수를 호출한다.
 */
void Server::runServer()
{
	struct timeval tv = { TIMEOUT_SEC, 0 };

	_read_fds_backup = _read_fds;
	_write_fds_backup = _write_fds;
	if (this->_is_debug)
	{
		std::cout << F_YELLOW << "[DEBUG] Server running..." << FB_DEFAULT << std::endl;
		std::cout << *this << std::endl;
	}
	int ready_descriptors = select(this->_fd_max + 1, &_read_fds_backup, &_write_fds_backup, NULL, &tv);
	if (ready_descriptors == -1)
		throw std::runtime_error("Error: select() failed");
	if (ready_descriptors == 0)
		return;
	for (int i = 0; i <= this->_fd_max; i++)
	{
		if (FD_ISSET(i, &_read_fds_backup))
		{
			if (i == this->_server_socket)
				this->acceptClient();
			else
			{
				std::string message = this->receiveMessage(i);
				std::vector<t_token> tokens;
				if (parseMessageFormat(message, this->_is_debug, tokens))
				{
					this->sendMessage(i, message);
					std::vector<std::string> vec = split_string(message);
					Connection connection;
					connection.socket_fd = i;
					handler.get_request(vec, connection);
				}
			}
		}
	}
}

/**
 * 클라이언트의 연결 요청을 수락하고, 클라이언트 소켓을 fd_set에 추가한다.
 */
void Server::acceptClient()
{
	socklen_t client_addr_len = sizeof(struct sockaddr_in);
	struct sockaddr client_addr;
	int client_socket = accept(this->_server_socket, &client_addr, &client_addr_len);
	if (client_socket == -1)
		throw std::runtime_error("Error: accept() failed");
	FD_SET(client_socket, &this->_read_fds);
	FD_SET(client_socket, &this->_write_fds);
	if (client_socket > this->_fd_max)
		this->_fd_max = client_socket;
	if (this->_is_debug)
		std::cout << F_YELLOW << "[DEBUG] New client connected: " << client_socket << FB_DEFAULT << std::endl;
}

/**
 * 클라이언트로부터 메시지를 받아온다. 사용 후에 메모리를 해제해야 한다.
 * @param client_socket 클라이언트 소켓
 * @return 클라이언트로부터 받은 메시지
 */

std::string Server::receiveMessage(int client_socket)
{
	std::vector<char> buffer(BUFFER_SIZE);
	ssize_t read_size = read(client_socket, &buffer[0], BUFFER_SIZE);
	if (read_size == -1)
		throw std::runtime_error("Error: read() failed");
	if (read_size == 0)
	{
		this->closeClient(client_socket);
		return "";
	}
	if (read_size < BUFFER_SIZE - 2)
		buffer.resize(read_size + 2);
	else
		buffer.resize(BUFFER_SIZE);
	if (this->_is_debug)
		std::cout << F_YELLOW << "[DEBUG] Message received: " << buffer.data() << FB_DEFAULT << std::endl;
	FD_SET(client_socket, &this->_read_fds);
	return std::string(buffer.begin(), buffer.end());
}

/**
 * 클라이언트 소켓을 닫고, fd_set에서 읽기용, 쓰기용 fd를 모두 제거한다.
 * @param client_socket
 */
void Server::closeClient(int client_socket)
{
	if (client_socket == this->_server_socket)
		return;
	if (client_socket > 0)
		close(client_socket);
	FD_CLR(client_socket, &this->_read_fds);
	FD_CLR(client_socket, &this->_write_fds);
	if (this->_is_debug)
		std::cout << F_YELLOW << "[DEBUG] Client closed: " << client_socket << FB_DEFAULT << std::endl;
}

/**
 * 클라이언트에게 메시지를 보낸다. 메시지를 모두 보내면 읽기용 fd는 fd_set에서 제거하고, 쓰기용 fd는 fd_set에 추가한다.
 * @param client_socket 메시지를 보낼 클라이언트 소켓
 * @param message 보낼 메시지
 */
void Server::sendMessage(int client_socket, std::string& message)
{
	ssize_t write_size = write(client_socket, message.data(), message.length());
	if (write_size == -1)
		throw std::runtime_error("Error: write() failed");
	if (write_size == static_cast<ssize_t >(message.length()))
	{
		FD_CLR(client_socket, &this->_read_fds);
		FD_SET(client_socket, &this->_write_fds);
	}
}

/**
 * 서버 소켓을 닫는다.
 */
void Server::closeServer()
{
	close(this->_server_socket);
}

std::ostream& operator<<(std::ostream& os, const fd_set& fds)
{
	os << "[ ";
	for (int i = 0; i < FD_SETSIZE; i++)
	{
		if (FD_ISSET(i, &fds))
		{
			os << i << " ";
		}
	}
	os << "]";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Server& server)
{
	struct sockaddr_in server_addr = server.getServerAddr();

	os << F_YELLOW << "[DEBUG] Server: " << FB_DEFAULT << std::endl;
	os << "=> Server socket     : " << server.getServerSocket() << std::endl;
	os << "=> Server address    : " << inet_ntoa(server_addr.sin_addr) << std::endl;
	os << "=> Server port       : " << ntohs(server_addr.sin_port) << std::endl;
	os << "=> Max Client Number : " << server.getMaxClientNumber() << std::endl;
	os << "=> FD_SET for recv() : " << server.getReadFds() << std::endl;
	os << "=> FD_SET for send() : " << server.getWriteFds() << std::endl;
	return os;
}
