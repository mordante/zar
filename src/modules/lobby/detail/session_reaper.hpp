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

#ifndef MODULES_LOBBY_DETAIL_SESSION_REAPER_HPP_INCLUDED
#define MODULES_LOBBY_DETAIL_SESSION_REAPER_HPP_INCLUDED

#include "modules/lobby/session.hpp"

#include <boost/asio/deadline_timer.hpp>

#include <list>

namespace lobby {

namespace detail {

class tsession_reaper final
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tsession_reaper(
			  boost::asio::io_service& io_service
			, std::list<tsession>& sessions__);

	~tsession_reaper() = default;

	tsession_reaper&
	operator=(const tsession_reaper&) = delete;
	tsession_reaper(const tsession_reaper&) = delete;

	tsession_reaper&
	operator=(tsession_reaper&&) = delete;
	tsession_reaper(tsession_reaper&&) = delete;


	/***** ***** Operators. ***** *****/

	/** Starts running the reaper. */
	void
	run();

private:

	/***** ***** Members. ***** *****/

	/**
	 * The list with the sessions with we manage.
	 *
	 * Periodically look in this list of sessions to see whether there are
	 * one or more candidates to reap.
	 */
	std::list<tsession>& sessions_;

	/** The timer used to implement the periodically reaping. */
	boost::asio::deadline_timer timer_;

	/** Reapse the session which are in the state @ref reapable. */
	void
	reap();

	/** The handler functor for @ref expires_from_now. */
	void
	wait_handler(const boost::system::error_code& error);
};

} // namespace detail

} // namespace lobby

#endif

