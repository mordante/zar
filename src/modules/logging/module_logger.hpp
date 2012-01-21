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
 * Contains the basic module logger class.
 */

#ifndef MODULES_LOGGING_MODULE_LOGGER_HPP_INCLUDED
#define MODULES_LOGGING_MODULE_LOGGER_HPP_INCLUDED

#include "modules/logging/level.hpp"

#include <boost/asio/strand.hpp>

#include <functional>
#include <map>

namespace logging {

class tmodule_logger final
{
public:

	/***** ***** Types. ***** *****/

	/** The function type for the \ref pre_processing stage. */
	typedef
			std::function<void(const std::string&, const tlevel, std::string&)>
			tpre_process_function;

	/** The function type for the \ref get_stream stage. */
	typedef
		std::function<std::ostream*(const std::string&, const tlevel)>
		tstream_function;

	/** The function type for the \ref post_processing stage. */
	typedef std::function<void(const std::string&)> tpost_process_function;


	/***** ***** Constructor, destructor, assignment. ***** *****/

	tmodule_logger() = default;
	~tmodule_logger();

	tmodule_logger&
	operator=(const tmodule_logger&) = delete;
	tmodule_logger(const tmodule_logger&) = delete;

	tmodule_logger&
	operator=(tmodule_logger&&) = delete;
	tmodule_logger(tmodule_logger&&) = delete;


	/***** ***** Operators. ***** *****/

	/**
	 * Registers a new module.
	 *
	 * If the module is already registered the function has no effect.
	 *
	 * @param module              The name of the module.
	 * @param threshold_level     The theshold level for the logging.
	 */
	void
	register_module(
			  const std::string& module
			, const tlevel threshold_level = tlevel::trace);

	/**
	 * Tests whether the @p level is active.
	 *
	 * A level is active if it's greater than or equal to
	 * @ref threshold_level_. The test is a test on the integral value of the
	 * @ref tlevel.
	 *
	 * @param level               The level to test.
	 *
	 * @returns                   Whether or not the @p level is active.
	 *                            Not registered modules are never active.
	 */
	bool
	is_active(const std::string& module, const tlevel level) const;

	/**
	 * Sets the asynchronous mode.
	 *
	 * @see @ref log() for more information.
	 *
	 * @warning The @p io_service shall remain a valid reference after this
	 * function terminates. Only after calling @ref set_sync_mode() the
	 * @p io_service is no longer used.
	 *
	 * @param io_service          The io_service used for dispatching.
	 */
	void
	set_async_mode(boost::asio::io_service& io_service);

	/**
	 * Sets the synchronous mode.
	 *
	 * @see @ref log() for more information.
	 */
	void
	set_sync_mode();

	/**
	 * Writes a message to the log.
	 *
	 * When calling @ref log() the following occurs:
	 *
	 * \li \anchor test_threshold @ref is_active() is called in the caller's
	 *     thread.
	 * \li \anchor pre_processing @ref pre_process_function_ is called in the
	 *     caller's thread.
	 * \li \anchor dispatch The logging is put in the logging queue storing the
	 *     current value of @ref stream_function_ and @ref
	 *     post_process_function_. (In synchronous mode the dispatching is also
	 *     done, but directly executed.)
	 *
	 * After the dispatching the real logger is called. This function gets the
	 * values stored during dispatching, especially @ref stream_function_ and
	 * @ref post_process_function_.
	 *
	 * During execution the following occurs, all in the thread that happens to
	 * execute the dispatched queue item.
	 *
	 * \li \anchor get_stream The stream to log is retrieved by calling
	 *     @ref stream_function_. If this function returns a @c nullptr the
	 *     code stops.
	 * \li \anchor write_to_stream The message is written to the stream.
	 * \li \anchor post_processing @ref post_process_function_ is called.
	 */
	void
	log(const std::string& module, const tlevel level, std::string message);


	/***** ***** Setters, getters. ***** *****/

	/** Sets the threshold level for all modules. */
	void
	set_threshold_level(const tlevel threshold_level);

	void
	set_threshold_level(
			  const std::string& module
			, const tlevel threshold_level);

	tlevel
	get_threshold_level(const std::string& module) const;

	boost::asio::io_service::strand*
	strand();

	void
	set_pre_process_function(const tpre_process_function& function);

	void
	set_stream_function(const tstream_function& function);

	void
	set_post_process_function(const tpost_process_function& function);

private:

	/***** ***** Members. ***** *****/

	/**
	 * The threshold for logging.
	 *
	 * @see @ref is_active() and @ref log().
	 */
	std::map<std::string, tlevel> modules_{};

	/**
	 * The strand to use in asynchronous mode.
	 *
	 * The basic idea of a strand is that all code in a strand is executed in
	 * order and no to items in the queue are executed at the same time. Two
	 * items in the queue after one another may be executed in different
	 * queues. The boost asio documentation explains more.
	 */
	boost::asio::io_service::strand* strand_{nullptr};

	/**
	 * The function used in the @ref pre_processing phase.
	 *
	 * @see @ref log() for more information.
	 */
	tpre_process_function pre_process_function_{};

	/**
	 * The function used in the @ref get_stream phase.
	 *
	 * @see @ref log() for more information.
	 */
	tstream_function stream_function_{std::bind(
			  &tmodule_logger::basic_stream_function
			, this
			, std::placeholders::_2)};

	/**
	 * The function used in the @ref post_processing phase.
	 *
	 * @see @ref log() for more information.
	 */
	tpost_process_function post_process_function_{};

	/**
	 * The basic function for getting a stream.
	 *
	 * This function is used upon creation the @ref tlogger. It logs to the
	 * @p std::cout and @p std::cerr depending on the @p level.
	 *
	 * @param level                  The level of the message to log.
	 *
	 * @returns                      The stream to log to.
	 */
	std::ostream*
	basic_stream_function(const tlevel level);
};

} // namespace logging

#endif
