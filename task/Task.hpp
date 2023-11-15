/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 00:14:43 by heshin            #+#    #+#             */
/*   Updated: 2023/11/16 00:55:25 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP
# include "../irc/IrcCommand.hpp"
# include "../user/User.hpp"
# include <string>
# include <vector>
# include <exception>
# include <memory>

struct Task: public Serializable {

	public:
		static std::auto_ptr<Task> create(std::vector<std::string>&, const Connection&);
		virtual bool has_error() const;
		const std::string& get_prefix() const;
		const Command get_command() const;	
		const Connection& get_connection() const;
		virtual std::string _get_label() const;
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
		UserTask(const Task& parent, const std::vector<std::string>&);
		User::Info info;

		virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;
		virtual std::string _get_label() const;

		UserTask& add_next(const UserTask&);
		bool is_ready() const;
};
#endif
