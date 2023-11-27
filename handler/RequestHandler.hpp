/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:26:09 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 02:15:20 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP
# include "../task/Task.hpp"
# include <string>
# include <vector>
# include <utility>

struct Connection;

class RequestHandler {
	
	public:
		RequestHandler(); 
		std::vector<std::pair<int, std::vector<std::string> > > 
			get_request(std::vector<std::string>&, const Connection&);
	private:
		std::vector<std::string> execute(const UserTask&);
		std::vector<std::pair<int, std::vector<std::string> > > 
			execute(ChannelTask&);
};
#endif
