/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 21:57:10 by heshin            #+#    #+#             */
/*   Updated: 2023/11/09 21:57:11 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERIALIZE_HPP
# define SERIALIZE_HPP
# include <string>
# include <vector>
# include <utility>
# include <ostream>
# include <map>
# include <list>
# include <vector>

class Serializable {

	public:
		virtual std::string _serialize(const int) const;
		virtual std::ostream& _add_to_serialization(std::ostream&, const int) const; 
		virtual std::vector<std::pair<std::string, const Serializable*> > _get_children() const;
		virtual std::string _get_label() const = 0;

};

template <typename T>
bool _is_serializable() {
	return false;
}

std::string _serialize(const std::map<std::string, const Serializable*>&, const int);
std::string _serialize(const std::vector<const Serializable*>&, const int);
std::string _serialize(const std::list<Serializable>&, const int);
#endif
