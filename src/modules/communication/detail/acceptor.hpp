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

#ifndef MODULES_COMMUNICATION_DETAIL_ACCEPTOR_HPP_INCLUDED
#define MODULES_COMMUNICATION_DETAIL_ACCEPTOR_HPP_INCLUDED

#include "modules/communication/detail/connection.hpp"

#include <boost/asio/ip/tcp.hpp>

namespace communication {

namespace detail {

/**
 * Class that accepts an incoming connection.
 *
 * @tparam IP                     The internet protocol to use.
 */
template<class IP>
class tacceptor
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	/**
	 * Constructor.
	 *
	 * @pre                       lifetime(connection__) > lifetime(*this)
	 * @pre                       lifetime(socket__) > lifetime(*this)
	 *
	 * @param connection__        The connection containing the settings for
	 *                            the acceptor.
	 * @param socket__            The socket used for the communication.
	 */
	tacceptor(tconnection& connection__, typename IP::socket& socket__);

	~tacceptor();

	tacceptor&
	operator=(const tacceptor&) = delete;
	tacceptor(const tacceptor&) = delete;

	tacceptor&
	operator=(tacceptor&&) = delete;
	tacceptor(tacceptor&&) = delete;


	/***** ***** Operators. ***** *****/

	/**
	 * Connects to a external host.
	 *
	 * Once the acceptor is done it calls its @ref accept_handler_.
	 *
	 * @param acceptor            The acceptor containing the acception
	 *                            settings.
	 */
	void
	accept(typename IP::acceptor& acceptor);


	/***** ***** Setters, getters. ***** *****/

	void
	set_accept_handler(const taccept_handler& accept_handler__);

private:

	/***** ***** Members. ***** *****/

	/** The settings for the connection. */
	tconnection& connection_;

	/** The socket used for the communication. */
	typename IP::socket& socket_;

	/** The user supplied functor to call after an acception attempt. */
	taccept_handler accept_handler_{};

	/**
	 * The handler functor for @ref IP::acceptor::async_accept.
	 */
	void
	asio_accept_handler(const boost::system::error_code& error);
};

extern template class tacceptor<boost::asio::ip::tcp>;
typedef tacceptor<boost::asio::ip::tcp> tacceptor_tcp_socket;

} // namespace detail

} // namespace communication

#endif
