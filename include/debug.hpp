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
#include <typeinfo>
#include <map>
#include <list>
#include <vector>

std::string getPortInDebugMode();

template<typename K, typename V>
std::string _map_value_type(const std::map<K, V>&) {
	return typeid(V).name();
}

template<typename K, typename V>
std::string _map_key_type(const std::map<K, V>&) {
	return typeid(K).name();
}

template<typename T>
std::string _list_element_type(const std::list<T>&) {
	return typeid(T).name();
}

template<typename T>
std::string _vector_element_type(const std::vector<T>&) {
	return typeid(T).name();
}
#endif //FT_IRC__DEBUG_HPP_
