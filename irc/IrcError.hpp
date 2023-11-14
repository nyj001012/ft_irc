/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 04:23:08 by heshin            #+#    #+#             */
/*   Updated: 2023/11/15 01:22:53 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_ERROR_HPP
# define IRC_ERROR_HPP

struct IrcError {

	enum {
		ERR_NEEDMOREPARAMS,
		ERR_ALREADYREGISTRED,
	} code;
};
#endif
