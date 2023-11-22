/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 17:50:03 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 01:31:43 by heshin           ###   ########.fr       */
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
		Channel(const std::string&, const User&);
		Channel(const Channel&);
		virtual ~Channel();
		const std::string& get_name() const;
		const std::string& get_topic() const;
		const User& get_operator() const;
		void set_key(const std::string&, const User&);
		const std::string& get_key() const;
		std::vector<const User*> get_users() const;
		std::vector<std::string> get_user_names() const;
		bool is_equal(const Channel&) const;
		void add_user(const User&);
		void remove_user(const User&);
		bool is_allowed_to_join(const User&);

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
		std::string name;
		std::string topic;
		std::string key;
		std::vector<const User*> users;
		const User* operator_user;

		Channel();
		Channel& operator=(const Channel&);
};

bool operator==(const Channel&, const Channel&);
#endif
