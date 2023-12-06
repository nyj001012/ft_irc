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
		std::vector<std::pair<int, std::string> > recipients;
		void add_recipient(std::pair<int, std::string>);
		std::vector<std::string> contents;
		std::string prefix;
		template <typename T> void foreach(T&, void (T::*func)(int, std::string&));
};
Message& add_new_message(const std::vector<std::string>&, const std::string&, const std::vector<std::pair<int, std::string> >&, std::vector<Message>&);
Message& add_new_message(const std::vector<std::string>&, const std::string&,  const std::pair<int, std::string>, std::vector<Message>&);
Message add_new_message(const std::vector<std::string>&, const std::vector<int>&, std::vector<Message>&);
Message add_new_message(const std::vector<std::string>&, const int, std::vector<Message>&);

template <typename T> void Message::foreach(T& t,void (T::*func)(int, std::string&)) {
	const std::set<std::pair<int, std::string> > targets(recipients.begin(), recipients.end());
	std::set<std::pair<int, std::string> >::iterator iter;
	for (iter = targets.begin(); iter != targets.end(); ++iter) {
		for (size_t j = 0; j < contents.size(); ++j) {
			std::string& content = contents[j];
			if (prefix.empty() && iter->second.empty()) {
				(t.*func)(iter->first, content);
				continue;
			}
			std::string result;
			if (!prefix.empty()) {
				result = ":" + prefix + ' ';
			}
			if (!iter->second.empty()) {
				result += iter->second + ' ';
			}
			result += ":" + content;
			(t.*func)(iter->first, result);
		}
	}
}

#endif
