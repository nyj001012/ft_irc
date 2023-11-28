/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 22:45:11 by heshin            #+#    #+#             */
/*   Updated: 2023/11/29 03:45:54 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# include <string>
# include <vector>
# include <set>

struct Message {
	public:
		static std::vector<int> get_all_fds(const std::vector<Message>&);
		static std::vector<Message> create_start_from(const int);
		std::vector<int> recipients_fd;
		std::vector<std::string> contents;
		template <typename T> void foreach(T&, void (T::*func)(int, std::string&));
		void remove_fd(const int);
};
Message& add_new_message(const std::vector<std::string>&, const std::vector<int>&, std::vector<Message>&);
Message& add_new_message(const std::vector<std::string>&, const int, std::vector<Message>&);

template <typename T> void Message::foreach(T& t,void (T::*func)(int, std::string&)) {
	const std::set<int> fds(recipients_fd.begin(), recipients_fd.end());
	std::set<int>::iterator iter;
	for (iter = fds.begin(); iter != fds.end(); ++iter) {
		for (size_t j = 0; j < contents.size(); ++j) {
			std::string& content = contents[j];
			(t.*func)(*iter, content);
		}
	}
}

#endif
