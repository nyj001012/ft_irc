/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_util.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 01:34:35 by heshin            #+#    #+#             */
/*   Updated: 2023/11/03 01:34:35 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"
#include <sstream>
#include <iterator>

using std::vector;
using std::string;
using std::stringstream;
using std::istringstream;
typedef std::istream_iterator<string> str_iter;

vector<string> split_string(const string& str) {
	stringstream ss(str);
	return vector<string>(str_iter(ss), str_iter());
}

vector<string> split_string(const string& str, const char del) {
	std::istringstream ss(str);
	vector<string> vec;
	string temp;
	while (getline(ss, temp, del)) {
		vec.push_back(temp);
		temp.clear();
	}
	return vec;
}
