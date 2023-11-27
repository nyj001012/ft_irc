/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcLabels.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:34:22 by heshin            #+#    #+#             */
/*   Updated: 2023/11/25 23:34:22 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_LABELS_HPP
# define IRC_LABELS_HPP
# include <cstddef>

namespace IRC {

	struct ChannelLabel {
		public:
			static const size_t NAME_MAX_LENGTH = 50;
			static const char LOCAL_CHANNEL_PREFIX = '&';
			static const char* NOT_ALLOWED_CHARS;
	};
};

#endif
