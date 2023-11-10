/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 21:42:41 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 21:45:13 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/json.hpp"

template <>
std::ostream& json(std::ostream&os, std::string& s) {
	return os << '"' << s << '"';
}

template <>
std::ostream& json(std::ostream&os, std::string s) {
	return os << '"' << s << '"';
}
