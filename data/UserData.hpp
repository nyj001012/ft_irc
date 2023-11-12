/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 03:41:50 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 19:14:01 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_DATA_HPP
# define USER_DATA_HPP
# include "../debug/Serializable.hpp"
# include <exception>
# include <map>
# include <string>
# include <vector>
# include <list>

struct Connection;
class User;

class UserData: public Serializable {

	public:
		static UserData& get_storage();
		~UserData();
		bool is_user_exist(const std::string&) const;
		User& get_user(const std::string&) const;
		User& create_user(const Connection Connection, const std::string& nick);
		void delete_user(const User&);
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
};
#endif
