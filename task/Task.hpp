/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 00:14:43 by heshin            #+#    #+#             */
/*   Updated: 2023/11/18 02:27:42 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP
# include "../include/Irc.hpp"
# include "../user/User.hpp"
# include <string>
# include <vector>
# include <exception>
# include <memory>

using IRC::Command;

struct Task: public Serializable {

	public:
		static std::auto_ptr<Task> create(std::vector<std::string>&, const Connection&);
		virtual bool has_error() const;
		const std::string& get_prefix() const;
		const Command get_command() const;	
		const Connection& get_connection() const;
		virtual std::string _get_label() const;
		virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;
		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
	
	struct ParsingFail: public std::exception {
		 virtual const char* what() const throw();
	};

	protected:
		Task();
		Task& operator=(const Task&);
		Connection connection;
		Command command;
		Task& set_prefix(const std::string&);
		std::string prefix;
};

struct UserTask: public Task {

	public:
		static bool is_valid_nickname(const std::string&);

		UserTask(const Task& parent, const std::vector<std::string>&);
		User::Info info;
		UserTask& add_next(const UserTask&);

		virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;
		virtual std::string _get_label() const;

};

struct ChannelTask: public Task {

	public:

		ChannelTask(const Task& parent, const std::vector<std::string>&);
		virtual std::string _get_label() const;
		std::vector<std::string> params; 
		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
};

#endif
