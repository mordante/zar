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
 * Base class for the communication module.
 */

#ifndef MODULES_COMMUNICATION_DETAIL_CONNECTION_HPP_INCLUDED
#define MODULES_COMMUNICATION_DETAIL_CONNECTION_HPP_INCLUDED

#include "lib/strand/strand.hpp"
#include "modules/communication/message.hpp"

namespace communication {

namespace detail {

class tconnection
	: public lib::tstrand
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tconnection() = default;

	~tconnection();

	tconnection&
	operator=(const tconnection&) = delete;
	tconnection(const tconnection&) = delete;

	tconnection&
	operator=(tconnection&&) = default;
	tconnection(tconnection&&) = default;


	/***** ***** Setters, getters. ***** *****/

	void
	set_protocol(const tprotocol protocol__);

	tprotocol
	get_protocol() const;

private:

	/***** ***** Members. ***** *****/

	/** The protocol used for the connection. */
	tprotocol protocol_{tprotocol::telnet};
};

} // namespace detail

} // namespace communication

#endif
