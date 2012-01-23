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

#include "modules/communication/detail/receiver.hpp"

#include "lib/exception/validate.tpp"
#include "modules/logging/log.hpp"

#include <boost/asio/completion_condition.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>

namespace communication {

namespace detail {

template<class STREAM>
treceiver<STREAM>::treceiver(tconnection& connection__, STREAM& stream__)
	: connection_(connection__)
	, stream_(stream__)
{
}

template<class STREAM>
treceiver<STREAM>::~treceiver() = default;

template<class STREAM>
void
treceiver<STREAM>::set_receive_handler(
		const treceive_handler& receive_handler__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	receive_handler_ = receive_handler__;
}

template<class STREAM>
void
treceiver<STREAM>::receive()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	connection_.strand_execute(std::bind(&treceiver::receive_message, this));
}

template<class STREAM>
void
treceiver<STREAM>::receive_message()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	switch(connection_.get_protocol()) {
		case tprotocol::direct :
			/*
			 * Need to use the transfer_all condition, instead of transfer
			 * exactly
			 */
			NOT_IMPLEMENTED_YET;
			return;

		case tprotocol::line :
			receive_until("\n");
			return;

		case tprotocol::telnet :
			receive_until("\r\n");
			return;

		case tprotocol::basic :
			receive(4, std::bind(
					&treceiver::asio_receive_handler_read_length
					, this
					, std::placeholders::_1
					, std::placeholders::_2));
			return;

		case tprotocol::compressed :
			NOT_IMPLEMENTED_YET;
			return;
	}

	ENUM_FAIL_RANGE(connection_.get_protocol());
}

template<class STREAM>
void
treceiver<STREAM>::receive_until(const std::string& terminator)
{
	LOG_T(__PRETTY_FUNCTION__, "' terminator '", terminator, "'.\n");

	typedef std::function<void(
				  const boost::system::error_code&
				, const size_t bytes_transferred
			)>
			thandler;

	auto functor = [&](thandler&& handler)
		{
			boost::asio::async_read_until(
					  stream_
					, input_buffer_
					, terminator
					, handler);
		};

	connection_.strand_execute(
			  functor
			, std::bind(
				  &treceiver::asio_receive_handler
				, this
				, std::placeholders::_1
				, std::placeholders::_2));
}

template<class STREAM>
void
treceiver<STREAM>::receive(
		  const size_t bytes
		, tasio_receive_handler receive_handler)
{
	LOG_T(__PRETTY_FUNCTION__, "' bytes '", bytes, "'.\n");

	auto functor = [&](tasio_receive_handler&& handler)
		{
			boost::asio::async_read(
					  stream_
					, input_buffer_
					, boost::asio::transfer_exactly(bytes)
					, handler);
		};

	connection_.strand_execute(functor, receive_handler);
}

template<class STREAM>
void
treceiver<STREAM>::asio_receive_handler_read_length(
		  const boost::system::error_code& error
		, const size_t bytes_transferred)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« bytes_transferred »" , bytes_transferred
			, "«.\n");

	total_bytes_transferred_ += bytes_transferred;

	if(error) {
		if(receive_handler_) {
			receive_handler_(error, bytes_transferred, nullptr);
		}
		input_buffer_.consume(bytes_transferred);
		receive();
		return;
	}

	VALIDATE(bytes_transferred == 4);

	const uint32_t length = network_buffer_to_host(
			boost::asio::buffer_cast<const char*>(input_buffer_.data()));

	input_buffer_.consume(bytes_transferred);

	receive(length, std::bind(
			  &treceiver::asio_receive_handler
			, this
			, std::placeholders::_1
			, std::placeholders::_2));
}

template<class STREAM>
void
treceiver<STREAM>::asio_receive_handler(
		  const boost::system::error_code& error
		, const size_t bytes_transferred)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« bytes_transferred »" , bytes_transferred
			, "«.\n");

	total_bytes_transferred_ += bytes_transferred;

	if(error) {
		if(receive_handler_) {
			receive_handler_(error, bytes_transferred, nullptr);
		}
		input_buffer_.consume(bytes_transferred);
		receive();
		return;
	}

	/* decode message */
	tmessage message(
			  connection_.get_protocol()
			, std::string(
				  boost::asio::buffer_cast<const char*>(input_buffer_.data())
				, bytes_transferred));

	input_buffer_.consume(bytes_transferred);

	if(receive_handler_) {
		receive_handler_(error, bytes_transferred, &message);
	}

	receive();
}

template class treceiver<boost::asio::ip::tcp::socket>;
template class treceiver<boost::asio::posix::stream_descriptor>;

} // namespace detail

} // namespace communication
