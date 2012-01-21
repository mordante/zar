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
 * Contains the basic logger class.
 *
 * @todo There also needs to be a tsimple_logger class.
 *
 * @todo Check how much code can be shared between the several logger
 * classes.
 */

#ifndef MODULES_LOGGING_LOGGER_HPP_INCLUDED
#define MODULES_LOGGING_LOGGER_HPP_INCLUDED

#include "lib/string/concatenate.tpp"
#include "modules/logging/level.hpp"

#include <boost/asio/strand.hpp>

#include <functional>

namespace logging {

/**
 * Basic logger class.
 *
 * This logger can be used in single-threaded and multi-threaded environments.
 * In the single threaded environment, both synchronous and asynchronous mode
 * can be used, in multi-threaded environment the asynchronous mode must be
 * used.
 *
 * Switching to asynchronous mode is done with @ref set_async_mode() this class
 * only puts the messages in the queue, the caller must make sure the queue is
 * also processed. (It's advisable to use the same @p io_service parameter as
 * the application uses for its normal processing.)
 *
 * Since the processing can happen asynchronous and in different threads
 * logging is explained in more detail for the multi-threaded asynchronous
 * mode. In the synchronous mode the order is the same but the context is
 * always the calling thread.
 *
 * @note When the programme stops the @p io_service the logging should switch
 * back to synchronous mode, from that moment on the logging is no longer
 * thread-safe.
 *
 * @warning The logging itself is thread-safe but it assumes that after
 * switching to asynchronous mode no other setters are called. It could be
 * fixed by adding mutexes to this module, but that seems overkill. If needed
 * it would be better to write a wrapper class that does the protection. An
 * example of a wrapper class is @ref logging::tmodule_logger. Another option
 * is to use @ref strand() and post the modification in the queue.
 */
class tlogger final
{
public:

	/***** ***** Types. ***** *****/

	/** The function type for the \ref pre_processing stage. */
	typedef
			std::function<void(const tlevel, std::string&)>
			tpre_process_function;

	/** The function type for the \ref get_stream stage. */
	typedef std::function<std::ostream*(const tlevel)> tstream_function;

	/** The function type for the \ref post_processing stage. */
	typedef std::function<void()> tpost_process_function;


	/***** ***** Constructor, destructor, assignment. ***** *****/

	tlogger() = default;
	~tlogger();

	tlogger&
	operator=(const tlogger&) = delete;
	tlogger(const tlogger&) = delete;

	tlogger&
	operator=(tlogger&&) = delete;
	tlogger(tlogger&&) = delete;


	/***** ***** Operators. ***** *****/

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
	 */
	bool
	is_active(const tlevel level) const;

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
	log(const tlevel level, std::string message);


	/***** ***** Setters, getters. ***** *****/

	void
	set_threshold_level(const tlevel threshold_level__);

	tlevel
	get_threshold_level() const;

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
	tlevel threshold_level_{tlevel::trace};

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
			  &tlogger::basic_stream_function
			, this
			, std::placeholders::_1)};

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
