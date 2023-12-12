/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcLabels.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejokim <sejokim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:34:22 by heshin            #+#    #+#             */
/*   Updated: 2023/12/04 17:58:10 by sejokim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_LABELS_HPP
# define IRC_LABELS_HPP
# include <cstddef>
# include <string>

namespace IRC {

	struct ChannelLabel {
		public:
			static const size_t NAME_MAX_LENGTH = 50;
			static const char LOCAL_CHANNEL_PREFIX = '&';
			static const char* NOT_ALLOWED_CHARS;
	};
	std::string get_lower_name(const std::string&);
};

#endif
