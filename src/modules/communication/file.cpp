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

/* Not really a file but only special file descriptors. */

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "communication"

#include "modules/communication/file.hpp"

namespace communication {

tfile::tfile(boost::asio::io_service& io_service, const int fd)
	: boost::asio::posix::stream_descriptor(io_service)
	, tconnection()
	, treceiver_file(io_service)
	, tsender_file(io_service)
{
	assign(dup(fd));
}

tfile::~tfile() = default;

} // namespace communication
