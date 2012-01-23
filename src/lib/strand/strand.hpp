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

#ifndef LIB_STRAND_STRAND_HPP_INCLUDED
#define LIB_STRAND_STRAND_HPP_INCLUDED

#include <boost/asio/strand.hpp>

namespace lib {

/**
 * The strand class offers a way to serialise execution.
 *
 * The class is a small wrapper for @ref boost::asio::io_service::strand
 * [1].
 *
 * [1]
 * http://www.boost.org/doc/libs/1_48_0/doc/html/boost_asio/overview/core/strands.html
 */
class tstrand
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tstrand() = default;

	virtual ~tstrand();

	tstrand&
	operator=(const tstrand&) = delete;
	tstrand(const tstrand&) = delete;

	tstrand&
	operator=(tstrand&&) = default;
	tstrand(tstrand&&) = default;


	/***** ***** Operators. ***** *****/

	/**
	 * Enables strand execution.
	 *
	 * @param io_service          The io_service to create the new strand
	 *                            in.
	 */
	void
	strand_enable(boost::asio::io_service& io_service);

	/**
	 * Enables strand execution.
	 *
	 * @param strand__            The strand to use.
	 */
	void
	strand_enable(boost::asio::io_service::strand& strand__);

	/** Disables strand execution. */
	void
	strand_disable();

	/**
	 * Executes code.
	 *
	 * Depending on whether or not the strand is disabled the code is either
	 * directly executed or in a strand context.
	 *
	 * @tparam FUNCTOR            The type of the functor. This can be a
	 *                            functor or a lambda function.
	 *
	 * @param functor             The code to execute.
	 */
	template<class FUNCTOR>
	void
	strand_execute(FUNCTOR&& functor)
	{
		if(strand_) {
			strand_->post(std::move(functor));
		} else {
			functor();
		}
	}

	/**
	 * Executes code.
	 *
	 * The code is always executed directly. Depending on whether or not the
	 * strand is disabled the @p handler is either directly executed or in a
	 * strand context.
	 *
	 * @tparam FUNCTOR            The type of the functor. This can be a
	 *                            functor or a lambda function. The type
	 *                            expects one parameter of the type
	 *                            @p HANDLER or the @p handler wrapped by
	 *                            boost asio's strand implementation.
	 * @tparam HANDLER            The type of the handler. This can be a
	 *                            functor. There seem to be issues with
	 *                            lambda functions, but not investigated
	 *                            further.
	 *
	 * @param functor             The code to execute.
	 * @param handler             The hander to execute.
	 */
	template<class FUNCTOR, class HANDLER>
	void
	strand_execute(FUNCTOR&& functor, HANDLER&& handler)
	{
		if(strand_) {
			functor(strand_->wrap(std::move(handler)));
		} else {
			functor(std::move(handler));
		}
	}

private:

	/***** ***** Members. ***** *****/

	/** The strand to use. */
	boost::asio::io_service::strand* strand_{nullptr};

	/**
	 * Do we onw the strand?
	 *
	 * Depending on the @ref strand_enable function called we either use a
	 * strand or create one. If we create on we're responsible for its
	 * destruction. So this flag keeps track of the state.
	 */
	bool own_strand_{false};

	/**
	 * Deletes the current strand.
	 *
	 * The function is a nop if @ref own_strand_ == @c false.
	 */
	void
	delete_strand();

};

} // namespace lib

#endif
