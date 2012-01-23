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

#ifndef MODULES_COMMUNICATION_TCP_SOCKET_HPP_INCLUDED
#define MODULES_COMMUNICATION_TCP_SOCKET_HPP_INCLUDED

#include "modules/communication/detail/acceptor.hpp"
#include "modules/communication/detail/connector.hpp"
#include "modules/communication/detail/receiver.hpp"
#include "modules/communication/detail/sender.hpp"

namespace communication {

class ttcp_socket final
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	explicit ttcp_socket(boost::asio::io_service& io_service);

	~ttcp_socket() = default;

	ttcp_socket&
	operator=(const ttcp_socket&) = delete;
	ttcp_socket(const ttcp_socket&) = delete;

	ttcp_socket&
	operator=(ttcp_socket&&) = delete;
	ttcp_socket(ttcp_socket&&) = delete;


	/***** ***** Operators. ***** *****/

	void
	strand_enable(boost::asio::io_service& io_service);

	void
	strand_enable(boost::asio::io_service::strand& strand__);

	void
	strand_disable();

	void
	accept(boost::asio::ip::tcp::acceptor& acceptor);

	void
	connect(const std::string& hostname, const std::string& service);

	void
	receive();

	uint32_t
	send_action(const std::string& message);

	void
	send_reply(const uint32_t id__, const std::string& message);

	void
	close();

	/***** ***** Setters, getters. ***** *****/

	void
	set_protocol(const tprotocol protocol__);

	tprotocol
	get_protocol() const;

	void
	set_accept_handler(const taccept_handler& handler__);

	void
	set_connect_handler(const tconnect_handler& handler__);

	void
	set_receive_handler(const treceive_handler& handler__);

	void
	set_send_handler(const tsend_handler& handler__);

private:

	/***** ***** Operators. ***** *****/

	detail::tconnection connection_;

	boost::asio::ip::tcp::socket socket_;

	detail::tacceptor_tcp_socket acceptor_;

	detail::tconnector_tcp_socket connector_;

	detail::treceiver_socket receiver_;

	detail::tsender_tcp_socket sender_;
};

} // namespace communication

#endif
