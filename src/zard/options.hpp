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
 * The command-line options for zard.
 */

#ifndef ZARD_OPTIONS_HPP_INCLUDED
#define ZARD_OPTIONS_HPP_INCLUDED

#include <string>

/**
 * A singleton class containing the command-line options for zard.
 *
 * Before the @ref toptions::options() can be called, @ref toptions::parse()
 * should be called once.
 */
struct toptions final
{
	/***** ***** Constructor, destructor, assignment. ***** *****/

private:

	toptions() = default;

public:

	~toptions() = default;

	toptions&
	operator=(const toptions&) = delete;
	toptions(const toptions&) = delete;

	toptions&
	operator=(toptions&&) = delete;
	toptions(toptions&&) = delete;

	/***** ***** Operators. ***** *****/

	/**
	 * Parses the command-line arguments.
	 *
	 * @warning This function shall be called no more than one time.
	 *
	 * @param argc                The @p argc parameter of @ref main().
	 * @param argv                The @p argv parameter of @ref main().
	 *
	 * @returns                   The options from the parsed command-line
	 *                            arguments.
	 */
	static const toptions&
	parse(int argc, char* argv[]);

	/**
	 * Returns the cached version of the parsed command-line arguments.
	 *
	 * @warning This function shall be called after @ref parse() is called.
	 *
	 * @returns                   The options from the cached version of the
	 *                            parsed command-line arguments.
	 */
	static const toptions&
	options();

	/***** ***** Members. ***** *****/

	/**
	 * Should the programme run in the foreground?
	 *
	 * If false the programme should run as daemon.
	 */
	bool foreground{false};

	/**
	 * The configuration file for the programme.
	 *
	 * If empty the configuration code will try the default location(s).
	 */
	std::string configuration_file{};

private:

	/***** ***** Operators. ***** *****/

	/**
	 * Factory to create a new singleton object.
	 *
	 * @param is_initialized      Flag to tell whether the singleton is
	 *                            already initialized when this function is
	 *                            called.
	 */
	static toptions&
	singleton(const bool is_initialized);
};

#endif
