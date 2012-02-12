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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "lobby"

#include "modules/lobby/detail/session_reaper.hpp"

#include "modules/logging/log.hpp"
#include "zard/configuration.hpp"

namespace lobby {

namespace detail {

tsession_reaper::tsession_reaper(
		  boost::asio::io_service& io_service
		, std::list<tsession>& sessions__)
	: sessions_(sessions__)
	, timer_(io_service)
{
}

void
tsession_reaper::run()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	timer_.expires_from_now(boost::posix_time::seconds(
			tconfiguration::configuration().reap_interval));

	timer_.async_wait(std::bind(
			  &tsession_reaper::wait_handler
			, this
			, std::placeholders::_1));
}

void
tsession_reaper::reap()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	auto itor = sessions_.begin();
	while(itor != sessions_.end()) {
		if(itor->get_status() == tsession::tstatus::reapable) {
			LOG_D("Session reaper: reaping.\n");
			itor = sessions_.erase(itor);
		} else {
			++itor;
		}
	}
}

void
tsession_reaper::wait_handler(const boost::system::error_code& error)
{
	LOG_T(__PRETTY_FUNCTION__, ": error »", error.message(), "«.\n");

	if(error != boost::asio::error::operation_aborted) {
		LOG_I("Session reaper: run.\n");
		reap();
		run();
	}
}

} // namespace detail

} // namespace lobby

