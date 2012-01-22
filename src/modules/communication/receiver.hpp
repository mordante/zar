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

#ifndef MODULES_COMMUNICATION_RECEIVER_HPP_INCLUDED
#define MODULES_COMMUNICATION_RECEIVER_HPP_INCLUDED

#include "modules/communication/connection.hpp"

#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>

namespace communication {

class treceiver_
	: public virtual tconnection
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	treceiver_();

	virtual ~treceiver_();

	treceiver_&
	operator=(const treceiver_&) = delete;
	treceiver_(const treceiver_&) = delete;

	treceiver_&
	operator=(treceiver_&&) = default;
	treceiver_(treceiver_&&) = default;


	/***** ***** Operators. ***** *****/

	void
	start(); /* rename to receive */


	/***** ***** Setters, getters. ***** *****/

	void
	set_receive_handler(const treceive_handler& handler__);

protected:

	/***** ***** Members. ***** *****/

	treceive_handler receive_handler_{};

	size_t total_bytes_transferred_{0};

	boost::asio::streambuf input_buffer_{};

	void
	async_receive_message();

	virtual void
	async_receive_until(const std::string& terminator) = 0;

	virtual void
	async_receive(const size_t bytes, thandler handler) = 0;

	void
	asio_receive_callback_read_length(
		  const boost::system::error_code& error
		, const size_t bytes_transferred);

	void
	asio_receive_callback(
		  const boost::system::error_code& error
		, const size_t bytes_transferred);
};

template<class AsyncReadStream>
class treceiver
	: virtual public AsyncReadStream
	, public treceiver_
{
public:

	explicit treceiver(boost::asio::io_service& io_service)
		: AsyncReadStream(io_service)
		, treceiver_()
	{
	}

	virtual ~treceiver() {} /* default gives linker errors. */

	treceiver&
	operator=(const treceiver&) = delete;
	treceiver(const treceiver&) = delete;

	treceiver&
	operator=(treceiver&&) = default;
	treceiver(treceiver&&) = default;

	virtual void
	async_receive_until(const std::string& terminator) override;

	virtual void
	async_receive(const size_t bytes, thandler handler) override;
};

extern template class treceiver<boost::asio::ip::tcp::socket>;
typedef treceiver<boost::asio::ip::tcp::socket> treceiver_socket;

extern template class treceiver<boost::asio::posix::stream_descriptor>;
typedef treceiver<boost::asio::posix::stream_descriptor> treceiver_file;

} // namespace communication

#endif

