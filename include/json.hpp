/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 21:41:12 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 21:43:03 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../debug/Serializable.hpp"
# include <algorithm>
# include <cstddef>
# include <exception>
# include <iostream>
# include <map>
# include <ostream>
# include <string>
# include <vector>

template <typename T>
bool _has_label() {
	return false;
}

template <typename T>
std::string _get_label(T &_t) {
	(void)_t;
	return "";
}

template <typename T>
std::ostream& _json(std::ostream& os, T t) {
	if (_has_label<T>())
		os << _get_label(t);
	else
		os << t;
	return os;
}

template <typename T>
std::ostream& _json(std::ostream& os, T* t) {
	return os << t;
}

template <typename T>
std::ostream& _json(std::ostream& os, const T* t) {
	return _json(os, const_cast<T*>(t));
}

template <typename T, typename U> 
std::ostream& _json(std::ostream& os, T t, U u){
	_json(os, t);
	return _json(os, u);
}

template <typename T, typename U, typename V> 
std::ostream& _json(std::ostream& os, T t, U u, V v){
	_json(os, t);
	_json(os, u);
	return _json(os, v);
}

template <typename T, typename U, typename V, typename X> 
std::ostream& _json(std::ostream& os, T t, U u, V v, X x){
	_json(os, t);
	_json(os, u);
	_json(os, v);
	return _json(os, x);
}

template <typename T> 
std::ostream& _json(std::ostream& os, const std::vector<T>& v) {
	os << "[";
	typename std::vector<T>::const_iterator iter;
	for (iter = v.begin(); iter != v.end(); ++iter) {
		_json(os, *iter);
		os << (iter + 1 == v.end() ? "]": ",");
	}
	return os;
}

template <typename T>
std::ostream& _json(std::ostream& os, const std::map<std::string, T>& map) {
	os << '{';
	typename std::map<std::string, T>::const_iterator iter = map.begin();
	while(true) {
		os <<'"' << iter->first << '"' << ':';
		_json(os, iter->second);
		if (++iter == map.end()) {
			 os << '}';
			 break;
		}
		os << ',';
	}
	return os;
}
