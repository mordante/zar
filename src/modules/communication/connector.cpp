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

#include "modules/communication/connector.hpp"
#include "modules/logging/log.hpp"

#include "lib/exception/validate.tpp"

namespace communication {

tconnector_::~tconnector_() = default;

void
tconnector_::set_connect_handler(const tconnect_handler& handler__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");
	connect_handler_ = handler__;
}

void
tconnector_::connect(const std::string& hostname, const std::string& service)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	if(boost::asio::io_service::strand* strand__ = strand()) {
		strand__->post(std::bind(
				  &tconnector_::async_query
				, this
				, hostname
				, service));
	} else {
		async_query(hostname, service);
	}
}

template class tconnector<boost::asio::ip::tcp>;

} // namespace communication
