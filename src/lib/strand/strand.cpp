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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "strand"

#include "lib/strand/strand.hpp"

#include "modules/logging/log.hpp"

namespace lib {

tstrand::~tstrand()
{
	delete_strand();
}

void
tstrand::strand_enable(boost::asio::io_service& io_service)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	delete_strand();
	strand_ = new boost::asio::io_service::strand(io_service);
	own_strand_ = true;
}

void
tstrand::strand_enable(boost::asio::io_service::strand& strand__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	delete_strand();
	strand_ = &strand__;
}

void
tstrand::strand_disable()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	delete_strand();
	own_strand_ = nullptr;
}

void
tstrand::delete_strand()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	if(own_strand_) {
		delete strand_;
		own_strand_ = false;
	}
}

} // namespace lib
