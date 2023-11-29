/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:23:09 by yena              #+#    #+#             */
/*   Updated: 2023/11/29 13:18:21 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_SERVER_INITIALIZE_HPP_
#define FT_IRC_SERVER_INITIALIZE_HPP_

#include "../include/common.hpp"
#include "../handler/RequestHandler.hpp"
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iomanip>
#include <unistd.h>
#include "../include/parse.hpp"

#define MAX_CLIENT_NUM 10
#define BUFFER_SIZE 512
#define TIMEOUT_SEC 5

class Server
{
public:
	Server();
	Server(const Server& server);
	Server& operator=(const Server& server);
	~Server();
	int getMaxClientNumber() const;
	int getServerSocket() const;
	fd_set getReadFds() const;
	fd_set getWriteFds() const;
	fd_set getReadFdsBackup() const;
	fd_set getWriteFdsBackup() const;
	int getFdMax() const;
	struct sockaddr_in getServerAddr() const;
	bool getIsDebug() const;
	void setMaxClientNumber(int max_client_number);
	void setServerSocket(int server_socket);
	void setReadFds(fd_set fds);
	void setWriteFds(fd_set fds);
	void setReadFdsBackup(fd_set fds);
	void setWriteFdsBackup(fd_set fds);
	void setFdMax(int fd_max);
	void setServerAddr(struct sockaddr_in server_addr);
	void setIsDebug(bool is_debug);
	void initializeServer(const char* port);
	void runServer();
	void acceptClient();
	void saveLineToBuffer(int client_socket);
	void receiveMessage(int client_socket);
	void sendMessage(int client_socket, std::string& message);
	void closeClient(int client_socket);
	void closeServer();
	std::string getWriteBuffer(int fd);
	std::string getReadBuffer(int fd);
	void setWriteBuffer(int fd, std::string buffer);
	void setReadBuffer(int fd, std::string buffer);
	void clearWriteBuffer(int fd);
	void clearReadBuffer(int fd);

private:
	int _max_client_number;
	int _server_socket;
	int _fd_max;
	struct sockaddr_in _server_addr;
	bool _is_debug;
	RequestHandler handler;
	fd_set _read_fds;
	fd_set _write_fds;
	fd_set _read_fds_backup;
	fd_set _write_fds_backup;
	std::map<int, Connection> _connections;
	std::map<int, std::string > _write_buffers;
	std::map<int, std::string > _read_buffers;
};
std::ostream& operator<<(std::ostream& os, const Server& server);
std::ostream& operator<<(std::ostream& os, const fd_set& fds);

#endif //FT_IRC_SERVER_INITIALIZE_HPP_
