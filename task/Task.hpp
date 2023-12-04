#ifndef TASK_HPP
# define TASK_HPP
# include "../include/Irc.hpp"
# include "../user/User.hpp"
# include <string>
#include <utility>
# include <vector>
# include <exception>
# include <memory>

using IRC::Command;

struct Task: public Serializable {

	public:
		static std::auto_ptr<Task> create(std::vector<std::string>&, const Connection&);
		const std::string& get_prefix() const;
		const Command get_command() const;	
		const Connection& get_connection() const;
		virtual bool has_error() const;
		virtual std::vector<std::string> get_reply() const;
		void add_error(const IRC::Error&);
		std::vector<IRC::Error> get_errors() const;

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
		std::vector<IRC::Error> errors;
		Task& set_prefix(const std::string&);
		std::string prefix;
};

struct MessageTask: public Task {

	public:
	enum RecipientType {
		CHANNEL,
		USER,
	};
	typedef std::pair<RecipientType, std::string> Recipient;
	MessageTask(const Task&, const std::vector<std::string>&);
	void set_sender(const User&);

	std::string content;
	std::vector<Recipient> recipients;
	virtual std::string _get_label() const;
	virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
	virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;

	protected:
	const User* sender;
};

struct UserTask: public Task {

	public:
		static bool is_valid_nickname(const std::string&);

		UserTask(const Task&, const std::vector<std::string>&);
		User::Info info;
		UserTask& add_next(const UserTask&);
		const std::vector<std::string>& get_params() const;
		virtual std::vector<std::string> get_reply() const;

		virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;
		virtual std::string _get_label() const;

	protected:
		std::vector<std::string> params;
};

struct ChannelTask: public Task {

	public:
		static std::string get_channel_join_message(const Channel&, const User&);
		static std::string get_channel_part_message(const Channel&, const User&, const std::string&);
		static bool is_valid_channel_name(const std::string&);

		ChannelTask(const Task&, const std::vector<std::string>&);
		virtual std::string _get_label() const;
		std::vector<std::string> params; 
		virtual std::vector<std::string> get_reply() const;
		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
		void add_channel_to_reply(const Channel&);

	private:
		std::vector<const Channel*> channels_to_reply;
};

#endif
