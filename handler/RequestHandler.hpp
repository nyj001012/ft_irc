/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:26:09 by heshin            #+#    #+#             */
/*   Updated: 2023/11/28 22:57:01 by heshin           ###   ########.fr       */
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
		std::vector<Message> 
			execute(UserTask&);
		std::vector<Message> 
			execute(ChannelTask&);
};
#endif
