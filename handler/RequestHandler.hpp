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
