/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:26:09 by heshin            #+#    #+#             */
/*   Updated: 2023/11/18 04:18:46 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP
# include "../task/Task.hpp"
# include <string>
# include <vector>

struct Connection;

class RequestHandler {
	
	public:
		RequestHandler(); 
		void get_request(std::vector<std::string>&, const Connection&);
	private:
		void execute(const UserTask&);
		void execute(const ChannelTask&);
};
#endif
