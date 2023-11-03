/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 17:50:03 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 17:50:03 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include <exception>
# include <vector>
# include <string>

class User;

class Channel {

	public:
		Channel(const std::string& name, const User& operator_user);
		Channel(const Channel&);
		~Channel();
		const std::string& get_name() const;
		const std::string& get_topic() const;
		const User& get_operator() const;
		std::vector<const User*> get_users() const;
		std::vector<std::string> get_user_names() const;
		bool operator==(const Channel&) const;
		void add_user(const User&);
		void remove_user(const User&);

		struct AlreadyJoined: std::exception {
			virtual const char * what();
		};

		struct NoPermission: std::exception {
			virtual const char * what();
		};
	
	private:
		std::vector<const User*> users;
		std::string name;
		std::string topic;
		const User* operator_user;

		Channel();
		Channel& operator=(const Channel&);
};

bool operator==(const Channel&, const Channel&);
#endif
