/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 03:41:50 by heshin            #+#    #+#             */
/*   Updated: 2023/12/04 13:13:33 by sejokim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_DATA_HPP
# define USER_DATA_HPP
# include "../debug/Serializable.hpp"
# include "../task/Task.hpp"
# include "../user/User.hpp"
# include <exception>
# include <map>
# include <string>
# include <vector>
# include <list>

class UserData: public Serializable {

	public:
		static UserData& get_storage();
		virtual ~UserData();
		bool is_user_exist(const std::string&) const;
		bool is_user_exist(const Connection&) const;
		User& get_user(const std::string&) const;
		User& get_user(const Connection&) const;
		User& create_user(const Connection Connection, const User::Info&);
		void delete_user(const User&);

		bool is_duplicated(const std::string&) const;
		bool is_pedding_user_exist(const Connection&) const;
		void add_pendding_user(const UserTask&);
		const UserTask& update_task(const UserTask&);
		void remove_task(const Connection&);
		void handle_connection_lost(const int);

		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
		virtual std::string _get_label() const;

	struct UserNotExist: public std::exception {
		virtual const char* what() const throw();
	};

	struct UserAlreadyExist: public std::exception {
		virtual const char* what() const throw();
	};

	private:
		UserData();
		std::list<User> users;
		std::map<std::string, std::list<User>::iterator> user_nick_map;
		std::map<int, std::list<User>::iterator> user_socket_map;
		std::map<int, UserTask> pendding_users;
};
#endif
