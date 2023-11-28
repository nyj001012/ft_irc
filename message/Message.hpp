/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 22:45:11 by heshin            #+#    #+#             */
/*   Updated: 2023/11/29 00:18:56 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# include <string>
# include <vector>

struct Message {
	public:
		std::vector<int> recipients_fd;
		std::vector<std::string> contents;
		static std::vector<int> get_all_fds(const std::vector<Message>&);
		template <typename T> void foreach(T&, void (T::*func)(int, std::string&));
};
void add_new_message(const std::vector<std::string>&, const std::vector<int>&, std::vector<Message>&);
void add_new_message(const std::vector<std::string>&, const int, std::vector<Message>&);

template <typename T> void Message::foreach(T& t,void (T::*func)(int, std::string&)) {
	for (size_t i = 0; i < recipients_fd.size(); ++i) {
		const int fd = recipients_fd[i];
		for (size_t j = 0; j < contents.size(); ++j) {
			std::string& content = contents[j];
			(t.*func)(fd, content);
		}
	}
}

#endif
