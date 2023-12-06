/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:26:09 by heshin            #+#    #+#             */
/*   Updated: 2023/12/05 16:15:44 by sejokim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP
# include "../task/Task.hpp"
# include <string>
# include <vector>
# include <utility>

struct Connection;
struct Message;

class RequestHandler {
	
	public:
		RequestHandler();
		std::vector<Message> 
			get_request(std::vector<std::string>&, const Connection&);
	private:
		std::vector<Message> execute(UserTask&);
		std::vector<Message> execute(ChannelTask&);
		std::vector<Message> execute(MessageTask&);
		std::vector<Message> execute(PingTask&);
};
#endif
