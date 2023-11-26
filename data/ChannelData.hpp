/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelData.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:02:20 by heshin            #+#    #+#             */
/*   Updated: 2023/11/23 00:48:13 by heshin           ###   ########.fr       */
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
		virtual ~ChannelData();
		bool is_channel_exist(const std::string&) const;
		Channel& get_channel(const std::string&) const;
		Channel& join_channel(const std::string&, const User&);
		Channel& join_channel(const std::string&, const std::string&, const User&);
		void leave_channel(const Channel&, const User&);
		void leave_all_joined_channels(const User&);
		void remove_channel(const Channel&);

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
		std::list<Channel> channels;
		std::map<std::string, const Channel*> channel_map;
};
#endif
