/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 02:20:54 by heshin            #+#    #+#             */
/*   Updated: 2023/11/25 20:36:41 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# include "../debug/Serializable.hpp"
# include <string>
# include <sys/socket.h>
# include <vector>
# include <ostream>

class Channel;

struct Connection: Serializable {

public:
	bool is_alive;

	enum {
		V4,
		V6
	} ip_family;
	std::string address;
	int port;
	int socket_fd;
	std::string password;
	virtual std::ostream& _add_to_serialization(std::ostream&, const int) const;
	virtual std::string _get_label() const;

	bool is_equal(const Connection&) const;
	Connection();
	Connection(const struct sockaddr_storage*, const int socket_fd);

	std::vector<char> getWriteBuffer();
	std::vector<char> getReadBuffer();
	void setWriteBuffer(std::vector<char>);
	void setReadBuffer(std::vector<char>);
	void clearWriteBuffer();
	void clearReadBuffer();

private:
	std::vector<char> _write_buffer;
	std::vector<char> _read_buffer;
};

class User: public Serializable {

	public:
		struct Info: public Serializable {
			std::string nick_name;
			std::string user_name;
			std::string host_name;
			std::string server_name;
			std::string real_name;
			std::string get_id() const;

			bool is_equal(const User::Info&) const;

			virtual std::ostream& _add_to_serialization(std::ostream&, const int) const;
			virtual std::string _get_label() const;
		};

		User(const Connection connection, const Info&);
		User(const User&);
		virtual ~User();

		bool is_available() const;
		const std::string& get_nickname() const;
		const Connection& get_connection() const;
		const Info& get_info() const;

		void add_channel(const Channel&);
		void remove_channel(const Channel&);
		void remove_channel(const std::string&);
		bool is_joined(const std::string&) const;
		std::vector<const Channel*> get_all_channels() const;
		bool is_equal(const User&) const;

	virtual std::ostream& _add_to_serialization(std::ostream&, const int) const;
	virtual std::string _get_label() const;
	virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;

	private:
		User();
		User& operator=(const User&);
		Connection connection;
		Info info;
		std::vector<const Channel*> joined_channels;
};

bool operator==(const User&, const User&);
bool operator==(const Connection&, const Connection&);
bool operator==(const User::Info&, const User::Info&);
#endif
