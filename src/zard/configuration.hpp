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
 * The configuration for zard.
 */

#ifndef ZARD_CONFIGURATION_HPP_INCLUDED
#define ZARD_CONFIGURATION_HPP_INCLUDED

#include "modules/logging/level.hpp"

#include <string>

/**
 * A singleton class containing the configuration options for zard.
 *
 * Before calling @ref tconfiguration::configuration()
 * @ref tconfiguration::parse() must be called.
 *
 * @note Not all configuration parameters are stored in this class some
 * parameters are read and directly applied.
 */
struct tconfiguration final
{
	/***** ***** Constructor, destructor, assignment. ***** *****/

private:

	tconfiguration() = default;

public:

	~tconfiguration() = default;

	tconfiguration&
	operator=(const tconfiguration&) = delete;
	tconfiguration(const tconfiguration&) = delete;

	tconfiguration&
	operator=(tconfiguration&&) = delete;
	tconfiguration(tconfiguration&&) = delete;

	/***** ***** Operators. ***** *****/

	/**
	 * Returns the configuration.
	 *
	 * On the first run it will do some initialisation and set some
	 * parameters.
	 */
	static const tconfiguration&
	configuration();

	/***** ***** Members. ***** *****/

	/**
	 * The number of threads to use.
	 *
	 * If the number is zero it tries to determine the number of cores and
	 * start one thread per core. If that fails it uses only one thread.
	 */
	unsigned threads{0};

	/** The port number the server listens to. */
	unsigned short port{2048};

	/**
	 * The interval between the running of the session reaper.
	 *
	 * The interval is in seconds.
	 */
	unsigned reap_interval{30};

private:

	/***** ***** Operators. ***** *****/

	/** Creates a new configuration object. */
	static tconfiguration&
	create();

	/**
	 * Validates the object and tries to fix some issues.
	 *
	 * Upon failure it will throw an exception.
	 */
	void
	validate();
};

#endif
