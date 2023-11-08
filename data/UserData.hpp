/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 03:41:50 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 03:41:50 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_DATA_HPP
# define USER_DATA_HPP
# include <exception>
# include <map>
# include <string>
# include <vector>
# include <list>

struct Connection;
class User;

class UserData {

	public:
		static UserData& get_storage();
		~UserData();
		bool is_user_exist(const std::string&) const;
		User& get_user(const std::string&) const;
		void add_user(const User&);
		void delete_user(const User&);

	struct UserNotExist: public std::exception {
		virtual const char* what();
	};

	struct UserAlreadyExist: public std::exception {
		virtual const char* what();
	};

	private:
		UserData();
		std::list<User> users;
		std::map<std::string, std::list<User>::iterator> user_nick_map;
};
#endif
