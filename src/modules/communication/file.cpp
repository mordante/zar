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
	: file_(io_service)
	, receiver_(connection_, file_)
	, sender_(connection_, file_)
{
	file_.assign(dup(fd));
}

tfile::~tfile() = default;

void
tfile::strand_enable(boost::asio::io_service& io_service)
{
	// SHOULD BE OUR OWN IO_SERVICE !!!
	connection_.strand_enable(io_service);
}

void
tfile::strand_enable(boost::asio::io_service::strand& strand__)
{
	connection_.strand_enable(strand__);
}

void
tfile::strand_disable()
{
	connection_.strand_disable();
}

void
tfile::receive()
{
	receiver_.receive();
}

uint32_t
tfile::send_action(const std::string& message)
{
	return sender_.send_action(message);
}

void
tfile::send_reply(const uint32_t id__, const std::string& message)
{
	sender_.send_reply(id__, message);
}

void
tfile::set_protocol(const tprotocol protocol__)
{
	connection_.set_protocol(protocol__);
}

tprotocol
tfile::get_protocol() const
{
	return connection_.get_protocol();
}

void
tfile::set_receive_handler(const treceive_handler& handler__)
{
	receiver_.set_receive_handler(handler__);
}

void
tfile::set_send_handler(const tsend_handler& handler__)
{
	sender_.set_send_handler(handler__);
}

} // namespace communication
