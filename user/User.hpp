/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 02:20:54 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 02:20:54 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# include <string>
# include <sys/socket.h>

struct Connection {

	bool is_alive;

	enum {
		V4,
		V6
	} ip_family;
	std::string address;
	int port;
	int socket_fd;
	bool operator==(const Connection&) const;

	Connection();
	Connection(const struct sockaddr_storage*, const int socket_fd);
};

class User {

	public:
		User(const Connection connection, const std::string& nick);
		User(const User&);
		~User();
		
		bool is_available() const;
		const std::string& get_nickname() const;
		const std::string& get_hostname() const;
		const Connection& get_connection() const;
		bool operator==(const User&) const;

	private:
		User();
		User& operator=(const User&);
		Connection connection;
		std::string nickname;
		std::string hostname;
};

bool operator==(const User&, const User&);
bool operator==(const Connection&, const Connection&);
#endif
