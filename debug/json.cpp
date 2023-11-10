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

using std::ostream;
using std::string;

template <>
ostream& _json(ostream& os, string& s) {
	return os << '"' << s << '"';
}

template <>
ostream& _json(ostream& os, string s) {
	return os << '"' << s << '"';
}

template <>
ostream& _json(ostream& os, char* s) {
	return _json(os, string(s)); 
}

template <>
ostream& _json(ostream& os, bool b) {
	return os << (b ? "true": "false");
}
