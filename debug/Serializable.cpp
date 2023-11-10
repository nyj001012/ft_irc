/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serializable.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:34:06 by heshin            #+#    #+#             */
/*   Updated: 2023/11/10 14:34:06 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serializable.hpp"
#include "../include/json.hpp"
#include <sstream>
#include <ostream>
#include <utility>
#include <vector>
#include <typeinfo>

using std::string;
using std::vector;
using std::ostream;
using std::stringstream;
using std::map;
using std::list;

typedef std::pair<string, const Serializable*> KeyValue;

template <>
bool _is_serializable<Serializable>() {
	return true;
}

template <>
bool _has_label<Serializable>() {
	return true;
}

template <>
string _get_label(Serializable &s) {
	return s._get_label();
}

std::vector<KeyValue> Serializable::_get_children() const {
	return std::vector<KeyValue>();
}

template <>
std::ostream& _json<Serializable>(std::ostream& os, Serializable* t) {
	return os << '{';
		_json(os, typeid(*t).name()) << ":" 
			<< t->_serialize(1) << '}';
}

string Serializable::_serialize(const int depth) const {
	if (depth > 0) {
		stringstream ss;
		ss << '{';
		vector<KeyValue> children = _get_children();
		if (children.size() > 0) {
			vector<KeyValue>::const_iterator iter;
			for (iter = children.begin(); iter != children.end(); ++iter) {
				ss << '"' + iter->first + "\":" 
					<< iter->second->_serialize(depth - 1);
				if (iter + 1 != children.end())
					ss << ',';
			}
		}
		_add_to_serialization(ss, depth - 1);
		ss << '}';
		return ss.str();
	}
	else
		return '"' + _get_label() + '"';
}

ostream& Serializable::_add_to_serialization(ostream& os, const int _d) const {
	(void)_d;
	return os;
}

string _serialize(const map<string, const Serializable*>&, const int) {
	return "";
}

string _serialize(const list<Serializable>&, const int) {
	return "";
}
