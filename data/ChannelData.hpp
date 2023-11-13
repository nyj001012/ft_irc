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
# include "../debug/Serializable.hpp"
# include "../task/Task.hpp"
# include <exception>
# include <map>
# include <string>
# include <vector>
# include <list>

class Channel;
class User;

class ChannelData: public Serializable {

	public:
		static ChannelData& get_storage();
		~ChannelData();
		bool is_channel_exist(const std::string&) const;
		Channel& get_channel(const std::string&) const;
		Channel& join_channel(const std::string& name, const User&);
		void leave_channel(const Channel& channel, const User&);

		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
		virtual std::string _get_label() const;

		struct ChannelNotExist: public std::exception { 
			virtual const char* what() const throw();
		};

		struct ChannelAlreadyExist: public std::exception {
			virtual const char* what() const throw();
		};

	private:
		ChannelData();
		Channel& create_channel(const std::string& name, const User&);
		void remove_channel(const Channel&);
		std::list<Channel> channels;
		std::map<std::string, const Channel*> channel_map;
};
#endif
