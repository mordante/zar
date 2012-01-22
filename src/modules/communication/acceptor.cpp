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

#include "modules/communication/acceptor.hpp"
#include "modules/logging/log.hpp"

#include "lib/exception/validate.tpp"

namespace communication {

tacceptor_::~tacceptor_() = default;

void
tacceptor_::set_accept_handler(const taccept_handler& handler__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");
	accept_handler_ = handler__;
}

void
tacceptor_::asio_accept_callback(const boost::system::error_code& error)
{
	if(accept_handler_) {
		accept_handler_(error);
	}
}

template<class InternetProtocol>
void
tacceptor<InternetProtocol>::accept(
		typename InternetProtocol::acceptor& acceptor)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");
	if(boost::asio::io_service::strand* strand__ = strand()) {
		acceptor.async_accept(
				  dynamic_cast<typename InternetProtocol::socket&>(*this)
				, strand__->wrap(std::bind(
					  &tacceptor::asio_accept_callback
					, this
					, std::placeholders::_1)));
	} else {
		acceptor.async_accept(
				  dynamic_cast<typename InternetProtocol::socket&>(*this)
				, std::bind(
					  &tacceptor::asio_accept_callback
					, this
					, std::placeholders::_1));
	}
}

template class tacceptor<boost::asio::ip::tcp>;

} // namespace communication
