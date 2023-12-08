/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 17:50:03 by heshin            #+#    #+#             */
/*   Updated: 2023/12/08 15:53:47 by sejokim          ###   ########.fr       */
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
		void	set_topic(const std::string &new_topic, const User &user);
		int get_number_of_users() const;
		const User& get_operator() const;
		const std::string& get_key() const;
		std::vector<const User*> get_users() const;
		std::vector<std::string> get_user_names() const;
		bool is_equal(const Channel&) const;
		void add_user(const User&);
		void remove_user(const User&);
		bool is_allowed_to_join(const User&) const;
		bool is_allowed_to_invite(const User&) const;
		bool is_operator(const User &user) const;
		bool is_invite_only() const;

		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
		virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;
		virtual std::string _get_label() const;

		void	set_invite_only(bool setting);
		void	set_topic_protection(bool setting);
		void	set_key(const std::string &new_key);
		void	set_user_limit(int limit);
		void	remove_user_limit();
		void	add_operator(const User &user);
		void	remove_operator(const User &user);
		bool	can_join() const;

		struct AlreadyJoined: std::exception {
			virtual const char * what() const throw();
		};

		struct NoPermission: std::exception {
			virtual const char * what() const throw();
		};

		bool	topic_protected;
		void	enable_operator_assignment(bool enable);
		bool	is_operator_assignment_enabled() const;
	
	private:
		std::string name;
		std::string topic;
		std::string key;
		std::vector<const User*> users;
		const User* operator_user;

		bool	invite_only;
		int	user_limit;
		bool	operator_assignment_enabled; // 운영자 지정 기능 활성화 / 비활성화

		Channel();
		Channel& operator=(const Channel&);
};

bool operator==(const Channel&, const Channel&);
#endif
