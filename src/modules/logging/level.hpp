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
 * Contains the log levels for the logging classes.
 */

#ifndef MODULES_LOGGING_LEVEL_HPP_INCLUDED
#define MODULES_LOGGING_LEVEL_HPP_INCLUDED

namespace logging {

/**
 * The kind of message to log.
 */
enum class tlevel
{
	/**
	 * Tracing message.
	 *
	 * These messages are used for tracing all functions in a module. Whether
	 * or not a module implements tracing logging is up to the module. This
	 * level is normally used by developers.
	 */
	  trace

	/**
	 * Debug messages.
	 *
	 * These message are used for debugging. This level is normally used by
	 * developers.
	 */
	, debug

	/**
	 * Normal informal messages.
	 *
	 * These messages give an rough indication what the programme does. This
	 * level is for the curious user.
	 */
	, information

	/**
	 * Warning messages.
	 *
	 * These messages give warnings about unexpected issues, which can be
	 * recovered easily. This level is for the normal user.
	 */
	, warning

	/**
	 * Error messages.
	 *
	 * These messages give warning about serious issues. When a message of this
	 * kind occurs the working of the programme might be hampered. It is not
	 * advised to disable this level.
	 */
	, error

	/**
	 * Fatal error messages.
	 *
	 * These messages give a warning about very serious issues. A part of the
	 * programme is failing and cannot recover. This does not per se mean the
	 * programme starts working, but it functionality will be hampered. These
	 * messages are always logged.
	 */
	, fatal
};

} // namespace logging

#include "lib/string/enumerate.tpp"

ENUM_DECLARE_STREAM_OPERATORS(logging::tlevel)

#endif
