/*const  ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 23:40:08 by heshin            #+#    #+#             */
/*   Updated: 2023/11/16 01:15:08 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "../irc/IrcError.hpp"
#include "../irc/IrcCommand.hpp"
#include "../data/UserData.hpp"
#include "../user/User.hpp"
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::auto_ptr;

RequestHandler::RequestHandler() {}
void RequestHandler::get_request(vector<string>& req, const Connection& connection) {

	auto_ptr<Task> task;
	try {
		task = Task::create(req, connection);	
	}
	catch(IrcError &e) {
		std::cerr << e.what() << std::endl;
	}
	UserTask* user_task = dynamic_cast<UserTask*>(task.get());
	if (user_task != NULL) {
		try {
			execute(*user_task);		
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

void RequestHandler::execute(const UserTask& task) {
	UserData& data = UserData::get_storage();
	switch (task.get_command().type) {
		case Command::PASS:
			if (!data.is_pedding_user_exist(task.get_connection()))
				data.add_pendding_user(task);
			else 
				data.update_task(task);
			break;
		case Command::NICK:
			if (data.is_duplicated(task.info.nick_name)) {
				// TODO: irc error
			}
			else
				data.update_task(task);
			break;
		case Command::USER: 
			{
				const UserTask& last_task = data.update_task(task);
				data.create_user(last_task.get_connection(), last_task.info);
				data.remove_task(last_task.get_connection());
				break;
			}
		default:
			throw Command::UnSupported();
	}
}
