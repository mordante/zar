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

class tstrand
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tstrand() = default;

	~tstrand();

	tstrand&
	operator=(const tstrand&) = delete;
	tstrand(const tstrand&) = delete;

	tstrand&
	operator=(tstrand&&) = default;
	tstrand(tstrand&&) = default;


	/***** ***** Operators. ***** *****/

	void
	strand_enable(boost::asio::io_service& io_service);

	void
	strand_enable(boost::asio::io_service::strand& strand__);

	void
	strand_disable();

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

	boost::asio::io_service::strand* strand_{nullptr};

	bool own_strand_{false};

	void
	delete_strand();

};

} // namespace lib

#endif
