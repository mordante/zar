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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "communication"

#include "modules/communication/socket.hpp"

namespace communication {

tsocket::tsocket(boost::asio::io_service& io_service)
	: boost::asio::ip::tcp::socket(io_service)
	, tconnection()
	, treceiver_socket(io_service)
	, tsender_socket(io_service)
	, tconnector_socket(io_service)
	, tacceptor_socket(io_service)
{
}

tsocket::~tsocket() = default;

} // namespace communication
