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
 * Base types for the communication module.
 */

#ifndef MODULES_COMMUNICATION_TYPES_HPP_INCLUDED
#define MODULES_COMMUNICATION_TYPES_HPP_INCLUDED

namespace communication {

/**
 * The communication protocol used.
 *
 * The module can use several protocols for its communication. This @c enum
 * holds all types available.
 */
enum class tprotocol
{
	/** No formatting. */
	  direct

	/** Line based on Linux. */
	, line

	/** Line based on telnet. */
	, telnet

	/** Data has its length prefixed. */
	, basic

	/** Data has its length prefixed and compressed. */
	, compressed
};

} // namespace communication

#include "lib/string/enumerate.tpp"

ENUM_DECLARE_STREAM_OPERATORS(::communication::tprotocol)

#endif
