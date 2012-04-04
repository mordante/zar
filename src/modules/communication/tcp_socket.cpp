/*
 * Copyright (C) 2012 by Mark de Wever <koraq@xs4all.nl>
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

#include "modules/communication/tcp_socket.hpp"

namespace communication {

ttcp_socket::ttcp_socket(boost::asio::io_service& io_service)
	: connection_()
	, socket_(io_service)
	, acceptor_(connection_, socket_)
	, connector_(connection_, socket_)
	, receiver_(connection_, socket_)
	, sender_(connection_, socket_)
{
}

void
ttcp_socket::strand_enable(boost::asio::io_service& io_service)
{
	// SHOULD BE OUR OWN IO_SERVICE !!!
	connection_.strand_enable(io_service);
}

void
ttcp_socket::strand_enable(boost::asio::io_service::strand& strand__)
{
	connection_.strand_enable(strand__);
}

void
ttcp_socket::strand_disable()
{
	connection_.strand_disable();
}

void
ttcp_socket::accept(boost::asio::ip::tcp::acceptor& acceptor)
{
	acceptor_.accept(acceptor);
}

void
ttcp_socket::connect(const std::string& hostname, const std::string& service)
{
	connector_.connect(hostname, service);
}

void
ttcp_socket::receive()
{
	receiver_.receive();
}

uint32_t
ttcp_socket::send_action(const std::string& message)
{
	return sender_.send_action(message);
}

void
ttcp_socket::send_reply(const uint32_t id__, const std::string& message)
{
	sender_.send_reply(id__, message);
}

void
ttcp_socket::close()
{
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	socket_.close();
}

void
ttcp_socket::set_protocol(const tprotocol protocol__)
{
	connection_.set_protocol(protocol__);
}

tprotocol
ttcp_socket::get_protocol() const
{
	return connection_.get_protocol();
}

void
ttcp_socket::set_accept_handler(const taccept_handler& handler__)
{
	acceptor_.set_accept_handler(handler__);
}

void
ttcp_socket::set_connect_handler(const tconnect_handler& handler__)
{
	connector_.set_connect_handler(handler__);
}

void
ttcp_socket::set_receive_handler(const treceive_handler& handler__)
{
	receiver_.set_receive_handler(handler__);
}

void
ttcp_socket::set_send_handler(const tsend_handler& handler__)
{
	sender_.set_send_handler(handler__);
}


} // namespace communication
