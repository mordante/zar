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

#include "modules/communication/detail/connector.hpp"

#include "modules/logging/log.hpp"

#include <boost/asio/connect.hpp>

namespace communication {

namespace detail {

template<class IP>
tconnector<IP>::tconnector(
		  tconnection& connection__
		, typename IP::socket& socket__)
	: connection_(connection__)
	, socket_(socket__)
{
}

template<class IP>
tconnector<IP>::~tconnector() = default;

template<class IP>
void
tconnector<IP>::set_connect_handler(const tconnect_handler& connect_handler__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	connect_handler_ = connect_handler__;
}

template<class IP>
void
tconnector<IP>::connect(const std::string& hostname, const std::string& service)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	connection_.strand_execute(std::bind(
			  &tconnector::asio_resolve
			, this
			, hostname
			, service));
}

template<class IP>
void
tconnector<IP>::asio_resolve(
		  const std::string& hostname
		, const std::string& service)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": hostname »", hostname
			, "« service »", service
			, "«.\n");

	query_ = typename IP::resolver::query(IP::v4(), hostname, service);

	typedef std::function<void(
				  const boost::system::error_code&
				, typename IP::resolver::iterator
			)>
			thandler;

	auto functor = [&](thandler&& handler)
		{
			resolver_.async_resolve(query_, handler);
		};

	connection_.strand_execute(
			  functor
			, std::bind(
				  &tconnector::asio_resolve_handler
				, this
				, std::placeholders::_1
				, std::placeholders::_2));
}

template<class IP>
void
tconnector<IP>::asio_resolve_handler(
		  const boost::system::error_code& error
		, typename IP::resolver::iterator iterator)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "«.\n");

	if(error) {
		if(connect_handler_) {
			connect_handler_(error);
			return;
		}
	}

	asio_connect(iterator);
}

template<class IP>
void
tconnector<IP>::asio_connect(typename IP::resolver::iterator iterator)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	typedef std::function<void(
				  const boost::system::error_code&
				, typename IP::resolver::iterator
			)>
			thandler;

	auto functor = [&](thandler&& handler)
		{
			boost::asio::async_connect(socket_, iterator, handler);
		};

	connection_.strand_execute(
			  functor
			, std::bind(
				  &tconnector::asio_connect_handler
				, this
				, std::placeholders::_1
				, iterator));
}

template<class IP>
void
tconnector<IP>::asio_connect_handler(
		  const boost::system::error_code& error
		, typename IP::resolver::iterator iterator)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "«.\n");

	if(!error) {
		if(connect_handler_) {
			connect_handler_(error);
		}
		return;
	}

	++iterator;
	if(iterator == typename IP::resolver::iterator()) {
		if(connect_handler_) {
			connect_handler_(error);
		}
		return;
	}

	asio_connect(iterator);
}

template class tconnector<boost::asio::ip::tcp>;

} // namespace detail

} // namespace communication
