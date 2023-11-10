/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 21:41:12 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 21:43:03 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <algorithm>
# include <cstddef>
# include <map>
# include <ostream>
# include <string>
# include <vector>

template <size_t N> struct Fixed_string {
	char str[N];
	explicit Fixed_string(const char (&s)[N]) {
		std::copy(str, std::end(str), s);
	}
};

template <typename T>
bool has_label() {
	return false;
}

template <typename T>
std::ostream& json(std::ostream& os, T t) {
	return os << t;
}

template <typename T> 
std::ostream& json(std::ostream& os, const std::vector<T>& v) {
	os << "[";
	typename std::vector<T>::const_iterator iter;
	for (iter = v.begin(); iter != v.end(); ++iter) {
		if (has_label<T>())
			os << iter->get_label();
		else
			json(os, *iter);
		os << (iter + 1 == v.end() ? "]": ",");
	}
	return os;
}

template <typename T>
std::ostream& json(std::ostream& os, const std::map<std::string, T>& map) {
	os << "{";
	typename std::map<std::string, T>::const_iterator iter = map.begin();
	while(true) {
		os <<'"' << iter->first << '"' << ":";
		json(os, iter->second);
		if (++iter == map.end()) {
			 os << "}";
			 break;
		}
		os << ",";
	}
	return os;
}
