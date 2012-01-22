/*
 * Copyright (C) 2011 - 2012 by Mark de Wever <koraq@xs4all.nl>
 * Part of the zar project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY.
 *
 * See the COPYING file for more details.
 */

#ifndef MODULES_COMMUNICATION_SOCKET_HPP_INCLUDED
#define MODULES_COMMUNICATION_SOCKET_HPP_INCLUDED

#include "modules/communication/receiver.hpp"
#include "modules/communication/sender.hpp"
#include "modules/communication/connector.hpp"
#include "modules/communication/acceptor.hpp"

namespace communication {

class tsocket final
	: public treceiver_socket
	, public tsender_socket
	, public tconnector_socket
	, public tacceptor_socket
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	explicit tsocket(boost::asio::io_service& io_service);

	virtual ~tsocket();

	tsocket&
	operator=(const tsocket&) = delete;
	tsocket(const tsocket&) = delete;

	tsocket&
	operator=(tsocket&&) = default;
	tsocket(tsocket&&) = default;
};

} // namespace communication

#endif
