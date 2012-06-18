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

#ifndef MODULES_COMMUNICATION_DETAIL_SENDER_HPP_INCLUDED
#define MODULES_COMMUNICATION_DETAIL_SENDER_HPP_INCLUDED

#include "modules/communication/detail/connection.hpp"

#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

#include <queue>

namespace communication {

namespace detail {

/**
 * Class that sends data over a stream.
 *
 * @tparam STREAM                 The stream type to use.
 */
template<class STREAM>
class tsender
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
	tsender(tconnection& connection__, STREAM& stream__);

	~tsender();

	tsender&
	operator=(const tsender&) = delete;
	tsender(const tsender&) = delete;

	tsender&
	operator=(tsender&&) = delete;
	tsender(tsender&&) = delete;


	/***** ***** Operators. ***** *****/

	/**
	 * Sends an action message.
	 *
	 * Once the connector is done it calls its @ref send_handler_.
	 *
	 * Multiple send messages can be queued.
	 *
	 * @param message             The data of the action message to send.
	 *
	 * @returns                   The allocated message id.
	 */
	uint32_t
	send_action(const std::string& message);

	/**
	 * Sends an reply message.
	 *
	 * Once the connector is done it calls its @ref send_handler_.
	 *
	 * Multiple send messages can be queued.
	 *
	 * @param id                  The id of the message to send.
	 * @param message             The data of the reply message to send.
	 */
	void
	send_reply(const uint32_t id, const std::string& message);


	/***** ***** Setters, getters. ***** *****/

	void
	set_send_handler(const tsend_handler& send_handler__);

private:

	/***** ***** Members. ***** *****/

	/** The settings for the connection. */
	tconnection& connection_;

	/** The stream used for the communication. */
	STREAM& stream_;

	/** The underlaying buffer used to send the data over the stream. */
	boost::asio::streambuf send_buffer_{};

	/**
	 * The queue with the messages.
	 *
	 * The @c front() message is send an new messages are pushed at the
	 * back.
	 */
	std::deque<tmessage> messages_{};

	/** The user supplied functor to call after a message is send. */
	tsend_handler send_handler_{};

	/** The counter for generating new message ids. */
	std::atomic<uint32_t> id_{0};

	/** The total number of bytes send over the @ref stream_. */
	size_t total_bytes_transferred_{0};

	/**
	 * Message send function.
	 *
	 * This function queues the message and then calls
	 * @ref send_queue_message (if not already sending).
	 *
	 * @param message             The message to put in the @ref messages_
	 *                            queue.
	 */
	void
	send_message(tmessage message);

	/** Sends the @c front() message in the @ref messages_ queue. */
	void
	send_queue_message();

	/** The handler functor for @ref send_queue_message. */
	void
	send_queue_message_handler(
			  const boost::system::error_code& error
			, const size_t bytes_transferred);
};

extern template class tsender<boost::asio::ip::tcp::socket>;
typedef tsender<boost::asio::ip::tcp::socket> tsender_tcp_socket;

extern template class tsender<boost::asio::posix::stream_descriptor>;
typedef tsender<boost::asio::posix::stream_descriptor> tsender_file;

} // namespace detail

} // namespace communication

#endif
