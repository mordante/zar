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

#ifndef MODULES_COMMUNICATION_DETAIL_CONNECTOR_HPP_INCLUDED
#define MODULES_COMMUNICATION_DETAIL_CONNECTOR_HPP_INCLUDED

#include "modules/communication/detail/connection.hpp"

#include <boost/asio/ip/tcp.hpp>

namespace communication {

namespace detail {

/**
 * Class that makes a connection to another service.
 *
 * @todo Avoid being called when still resolving, that should throw an busy
 * exception.
 *
 * @tparam IP                     The internet protocol to use.
 */
template<class IP>
class tconnector
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
	 *                            the connector.
	 * @param socket__            The socket used for the communication.
	 */
	tconnector(tconnection& connection__, typename IP::socket& socket__);

	~tconnector();

	tconnector&
	operator=(const tconnector&) = delete;
	tconnector(const tconnector&) = delete;

	tconnector&
	operator=(tconnector&&) = delete;
	tconnector(tconnector&&) = delete;


	/***** ***** Operators. ***** *****/

	/**
	 * Connects to a external host.
	 *
	 * Once the connector is done it calls its @ref connect_handler_.
	 *
	 * @note At the moment only the IPv4 protocol is supported.
	 *
	 * @warning Once a connect request is started no new one may be issued
	 * until the current one is finished.
	 *
	 * @param hostname            The name or IP address of the host to
	 *                            connect to.
	 * @param service             The service name or port number to connect
	 *                            on the host.
	 */
	void
	connect(const std::string& hostname, const std::string& service);


	/***** ***** Setters, getters. ***** *****/

	void
	set_connect_handler(const tconnect_handler& connect_handler__);

private:

	/***** ***** Members. ***** *****/

	/** The settings for the connection. */
	tconnection& connection_;

	/** The socket used for the communication. */
	typename IP::socket& socket_;

	/**
	 * The resolver object.
	 *
	 * Needed to resolve the connection.
	 */
	typename IP::resolver resolver_{socket_.get_io_service()};

	/**
	 * The query used to resolve the connection.
	 *
	 * @note Since asio doesn't list the required lifetime so make sure the
	 * lifetime is long enough.
	 */
	typename IP::resolver::query query_{"", ""};

	/** The user supplied functor to call after a connection attempt. */
	tconnect_handler connect_handler_{};

	/** Wrapper to call @ref IP::resolver::async_resolve in a strand. */
	void
	asio_resolve(const std::string& hostname, const std::string& service);

	/** The handler functor for @ref asio_resolve. */
	void
	asio_resolve_handler(
			  const boost::system::error_code& error
			, typename IP::resolver::iterator iterator);

	/** Wrapper to call @ref IP::resolver::async_connect in a strand. */
	void
	asio_connect(typename IP::resolver::iterator iterator);

	/** The handler functor for @ref asio_connect. */
	void
	asio_connect_handler(
			  const boost::system::error_code& error
			, typename IP::resolver::iterator iterator);
};

extern template class tconnector<boost::asio::ip::tcp>;
typedef tconnector<boost::asio::ip::tcp> tconnector_tcp_socket;

} // namespace detail

} // namespace communication

#endif
