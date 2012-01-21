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

/**
 * @file
 * Defines the base class for all exceptions.
 */

#ifndef LIB_EXCEPTION_EXCEPTION_HPP_INCLUDED
#define LIB_EXCEPTION_EXCEPTION_HPP_INCLUDED

#include <string>
#include <iostream>
#include <type_traits>

namespace lib {

struct texception final
{
	/***** ***** Types. ***** *****/

	/**
	 * The type of exception thrown.
	 *
	 * All exceptions have a sub-type. Their basic type is determined by the
	 * object id but the sub-type gives more information regarding the exact
	 * cause of the issue. This sub-type is one of the members of this
	 * enumerate.
	 */
	enum class ttype
	{
		/**
		 * The value given is invalid.
		 *
		 * This is a generic error code when a value given is not valid. It can
		 * be the wrong type, but also a range error.
		 */
		  invalid_value

		/**
		 * The resource is busy with another task.
		 *
		 * The resource exists and access would be granted, however the
		 * resource is busy with another task and not available. The action can
		 * be retried later.
		 */
		, busy

		/**
		 * The resource could not be accessed.
		 *
		 * Either no permission, or the resource doesn't exist. Retrying should
		 * not help.
		 */
		, no_access

		/**
		 * An action doesn't honour the protocol.
		 */
		, protocol_error

		/**
		 * There was an communication error with a network socket.
		 */
		, socket_error

		/**
		 * The operation does not exist yet.
		 *
		 * This is thrown when a non-existing operation is attempted. This
		 * exception is used when the operation will be implemented in the
		 * future.
		 */
		, not_implemented_yet

		/**
		 * An internal failure occurred.
		 *
		 * This means an internal assertion has failed. This exception should
		 * never be thrown.
		 */
		, internal_failure
	};


	/***** ***** Constructor, destructor, assignment. ***** *****/

	texception(const ttype type__, const std::string& message__);

	~texception() = default;

	texception&
	operator=(const texception&) = delete;
	texception(const texception&) = delete;

	texception&
	operator=(texception&&) = default;
	texception(texception&&) = default;


	/***** ***** Members. ***** *****/

	ttype type;

	std::string message;
};

} // namespace lib

#include "lib/string/enumerate.tpp"

ENUM_DECLARE_STREAM_OPERATORS(lib::texception::ttype)

#endif
