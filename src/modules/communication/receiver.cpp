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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "communication"

#include "modules/communication/receiver.hpp"
#include "modules/logging/log.hpp"

#include "lib/exception/validate.tpp"

namespace communication {

treceiver_::treceiver_()
	: tconnection()
{
}


treceiver_::~treceiver_() = default;

void
treceiver_::set_receive_handler(const treceive_handler& handler__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");
	receive_handler_ = handler__;
}

void
treceiver_::start()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	strand_execute(std::bind(&treceiver_::async_receive_message, this));
}

void
treceiver_::async_receive_message()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");
	switch(get_protocol()) {
		case tprotocol::direct :
			/*
			 * Need to use the transfer_all condition, instead of transfer
			 * exactly
			 */
			NOT_IMPLEMENTED_YET;
			return;

		case tprotocol::line :
			async_receive_until("\n");
			return;

		case tprotocol::telnet :
			async_receive_until("\r\n");
			return;

		case tprotocol::basic :
			async_receive(4, std::bind(
					&treceiver_::asio_receive_callback_read_length
					, this
					, std::placeholders::_1
					, std::placeholders::_2));
			return;

		case tprotocol::compressed :
			NOT_IMPLEMENTED_YET;
			return;
	}

	ENUM_FAIL_RANGE(get_protocol());
}

void
treceiver_::asio_receive_callback_read_length(
		  const boost::system::error_code& error
		, const size_t bytes_transferred)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error '", error
			, "' bytes_transferred '" , bytes_transferred
			, "'.\n");

	total_bytes_transferred_ += bytes_transferred;

	if(error) {
		if(receive_handler_) {
			tconnection::tmessage message;
			receive_handler_(error, bytes_transferred, message);
		}
		input_buffer_.consume(bytes_transferred);
		start();
		return;
	}

	VALIDATE(bytes_transferred == 4);

	const uint32_t length = network_buffer_to_host(
			boost::asio::buffer_cast<const char*>(input_buffer_.data()));

	input_buffer_.consume(bytes_transferred);

	async_receive(length, std::bind(
			  &treceiver_::asio_receive_callback
			, this
			, std::placeholders::_1
			, std::placeholders::_2));
}

void
treceiver_::asio_receive_callback(
		  const boost::system::error_code& error
		, const size_t bytes_transferred)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error '", error
			, "' bytes_transferred '" , bytes_transferred
			, "'.\n");

	total_bytes_transferred_ += bytes_transferred;

	if(error) {
		if(receive_handler_) {
			tconnection::tmessage message;
			receive_handler_(error, bytes_transferred, message);
		}
		input_buffer_.consume(bytes_transferred);
		start();
		return;
	}

	/* decode message */
	tconnection::tmessage message = decode(std::string(
			  boost::asio::buffer_cast<const char*>(input_buffer_.data())
			, bytes_transferred));

	input_buffer_.consume(bytes_transferred);

	if(receive_handler_) {
		receive_handler_(error, bytes_transferred, message);
	}

	start();
}


template<class AsyncReadStream>
void
treceiver<AsyncReadStream>::async_receive_until(const std::string& terminator)
{
	LOG_T(__PRETTY_FUNCTION__, "' terminator '", terminator, "'.\n");

	typedef std::function<void(
				  const boost::system::error_code&
				, const size_t bytes_transferred
			)>
			thandler;

	strand_execute(
			[&](thandler&& handler)
			  {
				  boost::asio::async_read_until(
						  dynamic_cast<AsyncReadStream&>(*this)
						, input_buffer_
						, terminator
						, handler);
			  }
			, std::bind(
				  &treceiver_::asio_receive_callback
				, this
				, std::placeholders::_1
				, std::placeholders::_2));
}

template<class AsyncReadStream>
void
treceiver<AsyncReadStream>::async_receive(
		  const size_t bytes
		, thandler handler)
{
	LOG_T(__PRETTY_FUNCTION__, "' bytes '", bytes, "'.\n");
	strand_execute(
			[&](thandler&& h)
			  {
				  boost::asio::async_read(
						  dynamic_cast<AsyncReadStream&>(*this)
						, input_buffer_
						, boost::asio::transfer_exactly(bytes)
						, h);
			  }
			, handler);
}

template class treceiver<boost::asio::ip::tcp::socket>;
template class treceiver<boost::asio::posix::stream_descriptor>;

} // namespace communication
