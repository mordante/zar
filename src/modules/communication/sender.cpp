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

#include "modules/communication/sender.hpp"

#include "modules/logging/log.hpp"

namespace communication {

tsender_::tsender_()
	: tconnection()
{
}

tsender_::~tsender_() = default;

uint32_t
tsender_::send_action(const std::string& message)
{
	LOG_T(__PRETTY_FUNCTION__, ": message »", message, "«.\n");

	uint32_t id__;
	while((id__ = ++id_) == 0) { /* NOTHING */ }

	strand_execute(std::bind(
			  &tsender_::async_send_message
			, this
			, encode(tmessage::ttype::action, id__, message)));

	return id__;
}

void
tsender_::send_reply(const uint32_t id__, const std::string& message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": id »", id__
			, "« message »", message
			, "«.\n");

	strand_execute(std::bind(
			  &tsender_::async_send_message
			, this
			, encode(tmessage::ttype::reply, id__, message)));
}

void
tsender_::set_send_handler(const tsend_handler& handler__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	send_handler_ = handler__;
}

void
tsender_::async_send_message(tmessage message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": id »", message.id
			, "« message »", message.message
			, "«.\n");

	const bool sending = !messages_.empty();
	messages_.push_back(std::move(message));

	if(!sending) {
		async_send_queue();
	}
}

void
tsender_::asio_send_callback(
	  const boost::system::error_code& error
	, const size_t bytes_transferred)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error
			, "« bytes_transferred »", bytes_transferred
			, "«.\n");

	total_bytes_transferred_ += bytes_transferred;

	if(send_handler_) {
		send_handler_(error, bytes_transferred, messages_.front());
	}

	messages_.pop_front();
	if(!messages_.empty()) {
		async_send_queue();
	}
}

template<class AsyncWriteStream>
void
tsender<AsyncWriteStream>::async_send_queue()
{
	LOG_T(__PRETTY_FUNCTION__
			, ": id »", messages_.front().id
			, "« data »", messages_.front().message
			, "«.\n");

	typedef std::function<void(
				  const boost::system::error_code&
				, const size_t bytes_transferred
			)>
			thandler;

	strand_execute(
			[&](thandler&& handler)
			  {
				  boost::asio::async_write(
						  dynamic_cast<AsyncWriteStream&>(*this)
						, boost::asio::buffer(messages_.front().message)
						, handler);
			  }
			, std::bind(
				  &tsender_::asio_send_callback
				, this
				, std::placeholders::_1
				, std::placeholders::_2));
}

template class tsender<boost::asio::ip::tcp::socket>;
template class tsender<boost::asio::posix::stream_descriptor>;

} // namespace communication
