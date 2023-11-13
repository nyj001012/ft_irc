/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 00:14:43 by heshin            #+#    #+#             */
/*   Updated: 2023/11/13 04:18:48 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP
# include "../irc/IrcCommand.hpp"
# include "../user/User.hpp"
# include <string>
# include <vector>
# include <exception>

struct Task: public Serializable {

	public:
		static Task create(std::vector<std::string>&);
		virtual bool has_error() const;
		const std::string& get_prefix() const;
		virtual std::string _get_label() const;
		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
	
	struct ParsingFail: public std::exception {
		 virtual const char* what() const throw();
	};

	protected:
		Task();
		Command command;
		Task& set_prefix(const std::string&);
		std::string prefix;
};

struct UserTask: public Task {

	public:
		UserTask(const Command command, const std::vector<std::string>&);
		Connection connection;
		User::Info info;
		virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;
		virtual std::string _get_label() const;

		UserTask& add_next(const UserTask&);
		bool is_ready() const;

	protected:
		Command command;
};
#endif
