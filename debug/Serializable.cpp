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
#include "../include/debug.hpp"
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

vector<KeyValue> Serializable::_get_children() const {
	return std::vector<KeyValue>();
}

string Serializable::_get_label() const {
	return typeid(*this).name();
}

template <>
ostream& _json<Serializable>(std::ostream& os, Serializable* t) {
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
					<< iter->second->_serialize(depth - 1)
					<< ',';
			}
		}
		_add_to_serialization(ss, depth);
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

string _serialize(const map<string, const Serializable*>& m, const int depth) {
	stringstream ss;
	if (depth > 0 && m.size() > 0) {
		ss << '{';
		typename std::map<std::string, const Serializable*>::const_iterator iter = m.begin();
		while (true) {
			ss << '"' << iter->first << '"' << ':';
			ss << iter->second->_serialize(depth);
			if (++iter == m.end()) {
				ss << '}';
				break;
			}
			ss << ',';
		}
	}
	else {
		ss << '"' << "map<string," + _map_value_type(m) + '>'
			<< "(size=" << m.size() << ')' << '"';
	}
	return ss.str();
}

string _serialize(const vector<const Serializable*>& vec, const int depth) {
	stringstream ss;

	if (depth > 0 && vec.size() > 0) {
		ss << '[';
		vector<const Serializable*>::const_iterator iter;
		for (iter = vec.begin(); iter != vec.end(); ++iter) {
			ss << (*iter)->_serialize(depth);
			if (iter + 1 == vec.end())
				ss << ']';
			else
				ss << ',';
		}
	}
	else {
		ss << '"' << "vector<" << _vector_element_type(vec) << "> (size="
			<< vec.size() << ')' << '"';
	}
	return ss.str();
}

string _serialize(const list<Serializable *>& li, const int depth) {
	stringstream ss;

	if (depth > 0 && li.size() > 0) {
		ss << '[';
		list<Serializable*>::const_iterator iter = li.begin();
		while (true) {
			ss << (*iter)->_serialize(depth);	
			if (++iter == li.end()) {
				ss << ']';
				break;
			}
			else
				ss << ',';
		}
	}
	else {
		ss << '"' << "list<" << _list_element_type(li) << "> (size="
			<< li.size() << ')' << '"';
	}
	return ss.str();
}
