/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 17:50:03 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 19:25:34 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "../debug/Serializable.hpp"
# include <exception>
# include <vector>
# include <string>

class User;

class Channel: public Serializable {

	public:
		Channel(const std::string& name, const User& operator_user);
		Channel(const Channel&);
		~Channel();
		const std::string& get_name() const;
		const std::string& get_topic() const;
		const User& get_operator() const;
		std::vector<const User*> get_users() const;
		std::vector<std::string> get_user_names() const;
		bool is_equal(const Channel&) const;
		void add_user(const User&);
		void remove_user(const User&);

		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
		virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;
		virtual std::string _get_label() const;

		struct AlreadyJoined: std::exception {
			virtual const char * what() const throw();
		};

		struct NoPermission: std::exception {
			virtual const char * what() const throw();
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
