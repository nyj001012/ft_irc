/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_server.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yena <yena@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:23:09 by yena              #+#    #+#             */
/*   Updated: 2023/10/30 22:27:00 by yena             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_SERVER_INITIALIZE_HPP_
#define FT_IRC_SERVER_INITIALIZE_HPP_

#include "common.hpp"
#include <sys/socket.h>
#include <netinet/in.h>

void initializeServer(const char *port);

#endif //FT_IRC_SERVER_INITIALIZE_HPP_
