/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 21:54:30 by yena              #+#    #+#             */
/*   Updated: 2023/11/09 20:09:06 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC__DEBUG_HPP_
#define FT_IRC__DEBUG_HPP_

#include "common.hpp"
#include "../include/json.hpp"
#include <typeinfo>
#include <map>
#include <list>
#include <vector>

std::string getPortInDebugMode();

template<typename K, typename V>
std::string _map_value_type(const std::map<K, V>&) {
	return typeid(V).name();
}
/*
template<typename K, typename V>
std::string _map_key_type(const std::map<K, V>& map) {
	if (_has_label<K>() && map.size() != 0) {
		return map.begin()._get_label();
	}
	return typeid(K).name();
}

template<typename T>
std::string _list_element_type(const std::list<T>& li) {
	if (_has_label<T>() && li.size() != 0) {
		return li.front()._get_label()();
	}
	return typeid(T).name();
}

template<typename T>
std::string _vector_element_type(const std::vector<T>& vec) {
	if (_has_label<T>() && vec.size() != 0) {
		return vec[0]._get_label();
	}
	return typeid(T).name();
}
*/

template<typename T>
std::string _vector_element_type(const std::vector<T*>& vec) {
	if (_has_label<T>() && vec.size() != 0) {
		return _get_label(vec[0]);
	}
	return typeid(T).name();
}
template<typename K, typename V>
std::string _map_key_type(const std::map<K, V*>& map) {
	if (_has_label<K>() && map.size() != 0) {
		return _get_label(map.begin());
	}
	return typeid(K).name();
}

template<typename T>
std::string _list_element_type(const std::list<T*>& li) {
	if (_has_label<T>() && li.size() != 0) {
		return _get_label(li.front());
	}
	return typeid(T).name();
}

#endif //FT_IRC__DEBUG_HPP_
