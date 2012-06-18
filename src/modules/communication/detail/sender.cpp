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

#include "modules/communication/detail/sender.hpp"

#include "modules/logging/log.hpp"

#include <boost/asio/write.hpp>

namespace communication {

namespace detail {

template<class STREAM>
tsender<STREAM>::tsender(tconnection& connection__, STREAM& stream__)
	: connection_(connection__)
	, stream_(stream__)
{
}

template<class STREAM>
tsender<STREAM>::~tsender() = default;

template<class STREAM>
uint32_t
tsender<STREAM>::send_action(const std::string& message)
{
	LOG_T(__PRETTY_FUNCTION__, ": message »", message, "«.\n");

	uint32_t id__;
	while((id__ = ++id_) == 0) { /* NOTHING */ }

	connection_.strand_execute(std::bind(
			  &tsender::send_message
			, this
			, tmessage(tmessage::ttype::action, id__, message)));

	return id__;
}

template<class STREAM>
void
tsender<STREAM>::send_reply(const uint32_t id, const std::string& message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": id »", id
			, "« message »", message
			, "«.\n");

	connection_.strand_execute(std::bind(
			  &tsender::send_message
			, this
			, tmessage(tmessage::ttype::reply, id, message)));
}

template<class STREAM>
void
tsender<STREAM>::set_send_handler(const tsend_handler& send_handler__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	send_handler_ = send_handler__;
}

template<class STREAM>
void
tsender<STREAM>::send_message(tmessage message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": id »", message.id()
			, "« message »", message.contents()
			, "«.\n");

	const bool sending = !messages_.empty();
	messages_.push_back(std::move(message));

	if(!sending) {
		send_queue_message();
	}
}

template<class STREAM>
void
tsender<STREAM>::send_queue_message_handler(
	  const boost::system::error_code& error
	, const size_t bytes_transferred)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« bytes_transferred »", bytes_transferred
			, "«.\n");

	total_bytes_transferred_ += bytes_transferred;

	if(send_handler_) {
		send_handler_(error, bytes_transferred, messages_.front());
	}

	messages_.pop_front();
	if(!messages_.empty()) {
		send_queue_message();
	}
}

template<class STREAM>
void
tsender<STREAM>::send_queue_message()
{
	LOG_T(__PRETTY_FUNCTION__
			, ": id »", messages_.front().id()
			, "« data »", messages_.front().contents()
			, "«.\n");

	std::ostream os(&send_buffer_);
	os << messages_.front().encode(connection_.get_protocol());

	typedef std::function<void(
				  const boost::system::error_code&
				, const size_t bytes_transferred
			)>
			thandler;

	auto functor = [&](thandler&& handler)
		{
			boost::asio::async_write(
					  stream_
					, send_buffer_
					, handler);
		};

	connection_.strand_execute(
			  functor
			, std::bind(
				  &tsender::send_queue_message_handler
				, this
				, std::placeholders::_1
				, std::placeholders::_2));
}

template class tsender<boost::asio::ip::tcp::socket>;
template class tsender<boost::asio::posix::stream_descriptor>;

} // namespace detail

} // namespace communication
