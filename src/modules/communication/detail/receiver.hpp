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

#ifndef MODULES_COMMUNICATION_DETAIL_RECEIVER_HPP_INCLUDED
#define MODULES_COMMUNICATION_DETAIL_RECEIVER_HPP_INCLUDED

#include "modules/communication/detail/connection.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/streambuf.hpp>

namespace communication {

namespace detail {

/**
 * Class that receives data over a stream.
 *
 * @tparam STREAM                 The stream type to use.
 */
template<class STREAM>
class treceiver
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	/**
	 * Constructor.
	 *
	 * @pre                       lifetime(connection__) > lifetime(*this)
	 * @pre                       lifetime(stream__) > lifetime(*this)
	 *
	 * @param connection__        The connection containing the settings for
	 *                            the connector.
	 * @param stream__            The stream used for communication.
	 */
	treceiver(tconnection& connection__, STREAM& stream__);

	~treceiver();

	treceiver&
	operator=(const treceiver&) = delete;
	treceiver(const treceiver&) = delete;

	treceiver&
	operator=(treceiver&&) = delete;
	treceiver(treceiver&&) = delete;


	/***** ***** Operators. ***** *****/

	/**
	 * Starts receiving messages.
	 *
	 * For every message received @ref receive_handler_ is called. The when
	 * there is no error the callback function will automatically call
	 * receive again. This results in a continues receive process.
	 */
	void
	receive();


	/***** ***** Setters, getters. ***** *****/

	void
	set_receive_handler(const treceive_handler& receive_handler__);

private:

	/***** ***** Types. ***** *****/

	typedef std::function<void(
				  const boost::system::error_code& error
				, const size_t bytes_transferred
			)>
			tasio_receive_handler;


	/***** ***** Members. ***** *****/

	/** The settings for the connection. */
	tconnection& connection_;

	/** The stream used for the communication. */
	STREAM& stream_;

	/** The user supplied functor to call after a message is received. */
	treceive_handler receive_handler_{};

	/** The total number of bytes received over the @ref stream_. */
	size_t total_bytes_transferred_{0};

	/** Buffer to store the incoming stream data. */
	boost::asio::streambuf input_buffer_{};

	/**
	 * Receives a message.
	 *
	 * Depending on the protocol it calls a helper function to receive the
	 * stream data for the message.
	 */
	void
	receive_message();

	/**
	 * Receive data until a terminator is found.
	 *
	 * @param terminator          The terminator where to stop receiving.
	 *                            The terminator is received as part of the
	 *                            data.
	 */
	void
	receive_until(const std::string& terminator);


	/**
	 * Receive data until a number of bytes is transferred.
	 *
	 * Some protocols have a length prefix. The prefix has a fixed size
	 * followed by this number of bytes. So the message needs to be gathered
	 * in two steps. To differentiate between what is received the callback
	 * function @p asio_receive_handler differs.
	 *
	 * @param bytes               The number of bytes to transfer.
	 * @param asio_receive_handler
	 *                            The handler to call after receiving data.
	 */
	void
	receive(const size_t bytes, tasio_receive_handler receive_handler);

	/**
	 * A handler functor for @ref receive.
	 *
	 * This version is called when @ref receive is determine the size of the
	 * message.
	 */
	void
	asio_receive_handler_read_length(
		  const boost::system::error_code& error
		, const size_t bytes_transferred);

	/**
	 * A handler functor for @ref receive and @ref receive_until.
	 *
	 * This is always called for @ref receive_until and for @ref receive
	 * after the length has been determined.
	 */
	void
	asio_receive_handler(
		  const boost::system::error_code& error
		, const size_t bytes_transferred);
};

extern template class treceiver<boost::asio::ip::tcp::socket>;
typedef treceiver<boost::asio::ip::tcp::socket> treceiver_socket;

extern template class treceiver<boost::asio::posix::stream_descriptor>;
typedef treceiver<boost::asio::posix::stream_descriptor> treceiver_file;

} // namespace detail

} // namespace communication

#endif
