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

#ifndef MODULES_COMMUNICATION_SENDER_HPP_INCLUDED
#define MODULES_COMMUNICATION_SENDER_HPP_INCLUDED

#include "modules/communication/connection.hpp"

#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>

#include <queue>

namespace communication {

class tsender_
	: public virtual tconnection
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tsender_();

	virtual ~tsender_();

	tsender_&
	operator=(const tsender_&) = delete;
	tsender_(const tsender_&) = delete;

	tsender_&
	operator=(tsender_&&) = default;
	tsender_(tsender_&&) = default;


	/***** ***** Operators. ***** *****/

	uint32_t
	send_action(const std::string& message);

	void
	send_reply(const uint32_t id__, const std::string& message);


	/***** ***** Setters, getters. ***** *****/

	void
	set_send_handler(const tsend_handler& handler__);


protected:

	/***** ***** Members. ***** *****/

	std::deque<tmessage> messages_{};

	tsend_handler send_handler_{};

	std::atomic<uint32_t> id_{0};

	size_t total_bytes_transferred_{0};

	void
	async_send_message(tmessage message);

	virtual void
	async_send_queue() = 0;

	void
	asio_send_callback(
		  const boost::system::error_code& error
		, const size_t bytes_transferred);
};

template<class AsyncWriteStream>
class tsender
	: virtual public AsyncWriteStream
	, public tsender_
{
public:

	explicit tsender(boost::asio::io_service& io_service)
		: AsyncWriteStream(io_service)
		, tsender_()
	{
	}

	virtual ~tsender() {} /* default gives linker errors. */

	tsender&
	operator=(const tsender&) = delete;
	tsender(const tsender&) = delete;

	tsender&
	operator=(tsender&&) = default;
	tsender(tsender&&) = default;

	virtual void
	async_send_queue() override;
};

extern template class tsender<boost::asio::ip::tcp::socket>;
typedef tsender<boost::asio::ip::tcp::socket> tsender_socket;

extern template class tsender<boost::asio::posix::stream_descriptor>;
typedef tsender<boost::asio::posix::stream_descriptor> tsender_file;

} // namespace communication

#endif

