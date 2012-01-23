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

#include "modules/communication/detail/acceptor.hpp"

#include "modules/logging/log.hpp"

namespace communication {

namespace detail {

template<class IP>
tacceptor<IP>::tacceptor(
		  tconnection& connection__
		, typename IP::socket& socket__)
	: connection_(connection__)
	, socket_(socket__)
{
}

template<class IP>
tacceptor<IP>::~tacceptor() = default;

template<class IP>
void
tacceptor<IP>::accept(typename IP::acceptor& acceptor)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	typedef std::function<void(
				  const boost::system::error_code&
			)>
			thandler;

	auto functor = [&](thandler&& handler)
		{
			acceptor.async_accept(socket_, handler);
		};

	connection_.strand_execute(
			  functor
			, std::bind(
				  &tacceptor::asio_accept_handler
				, this
				, std::placeholders::_1));
}

template<class IP>
void
tacceptor<IP>::set_accept_handler(const taccept_handler& accept_handler__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	accept_handler_ = accept_handler__;
}

template<class IP>
void
tacceptor<IP>::asio_accept_handler(const boost::system::error_code& error)
{
	LOG_T(__PRETTY_FUNCTION__, ": error »", error.message(), "«.\n");

	if(accept_handler_) {
		accept_handler_(error);
	}
}

template class tacceptor<boost::asio::ip::tcp>;

} // namespace detail

} // namespace communication
