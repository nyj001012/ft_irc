/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reflector.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 09:56:49 by heshin            #+#    #+#             */
/*   Updated: 2023/11/12 09:56:49 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REFLECTOR_HPP
# define REFLECTOR_HPP
# include "Serializable.hpp"
# include <fstream>
# include <vector>
# include <utility>
# include <string>

class Reflector {

	public:
		static Reflector& shared();
		~Reflector();
		void add(const Serializable&);
		void update(const std::string&);
		void update();
		void set_depth(const int);
		void set_debug(bool);

	private:
		Reflector();
		std::ofstream outfile;
		int depth;
		std::string message;
		std::vector<std::pair<const Serializable*, std::string> > targets;
		std::string generate() const;
		bool is_debug;
};
#endif
