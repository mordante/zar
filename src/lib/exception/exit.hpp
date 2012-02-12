/*
 * Copyright (C) 2012 by Mark de Wever <koraq@xs4all.nl>
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

/**
 * @file
 * Defines exception class to terminate a programme.
 */

#ifndef LIB_EXCEPTION_EXIT_HPP_INCLUDED
#define LIB_EXCEPTION_EXIT_HPP_INCLUDED

namespace lib {

/**
 * Exception class to terminate the programme.
 *
 * When this exception it caught it's expected to terminate the programme
 * with an exit status of @ref texit.status.
 */
struct texit final
{
	/***** ***** Constructor, destructor, assignment. ***** *****/

	texit(const int status__);

	~texit() = default;

	texit&
	operator=(const texit&) = delete;
	texit(const texit&) = delete;

	texit&
	operator=(texit&&) = default;
	texit(texit&&) = default;


	/***** ***** Members. ***** *****/

	/** The status to exit the programme with. */
	int status;
};

} // namespace lib

#endif
