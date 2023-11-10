/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 02:20:54 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 21:53:37 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# include <string>
# include <sys/socket.h>
# include <vector>

class Channel;

struct Connection {

	bool is_alive;

	enum {
		V4,
		V6
	} ip_family;
	std::string address;
	int port;
	int socket_fd;

	bool is_equal(const Connection&) const;
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

		void add_channel(const Channel&);
		void remove_channel(const Channel&);
		void remove_channel(const std::string&);
		bool is_joined(const std::string&) const;
		std::vector<const Channel*> get_all_channels() const;
		bool is_equal(const User&) const;

		std::string get_label() const;

	private:
		User();
		User& operator=(const User&);
		Connection connection;
		std::string nickname;
		std::string hostname;
		std::vector<const Channel*> joined_channels;
};

bool operator==(const User&, const User&);
bool operator==(const Connection&, const Connection&);
#endif
