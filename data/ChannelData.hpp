/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelData.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:02:20 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 19:14:26 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_DATA_HPP
# define CHANNEL_DATA_HPP
# include <exception>
# include <map>
# include <string>
# include <vector>
# include <list>

class Channel;
class User;

class ChannelData{

	public:
		static ChannelData& get_storage();
		~ChannelData();
		bool is_channel_exist(const std::string&) const;
		Channel& get_channel(const std::string&) const;
		void join_channel(const std::string& name, const User&);
		void leave_channel(const Channel& channel, const User&);

		struct ChannelNotExist: public std::exception { 
			virtual const char* what() const throw();
		};

		struct ChannelAlreadyExist: public std::exception {
			virtual const char* what() const throw();
		};

	private:
		ChannelData();
		void create_channel(const std::string& name, const User&);
		void remove_channel(const Channel&);
		std::list<Channel> channels;
		std::map<std::string, std::list<Channel>::iterator> channel_map;
};
#endif
