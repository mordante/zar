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
 *
 * The module is still under heavy development and the class hierarchy has
 * not yet been cast in stone.
 *
 * @todo Finish and document the communication module.
 */

#ifndef MODULES_COMMUNICATION_CONNECTION_HPP_INCLUDED
#define MODULES_COMMUNICATION_CONNECTION_HPP_INCLUDED

#include "lib/strand/strand.hpp"
#include "modules/communication/message.hpp"

namespace communication {

class tconnection
	: public lib::tstrand
{
public:

	/***** ***** Types. ***** *****/

	typedef std::function<void(
				  const boost::system::error_code& error
			)>
			taccept_handler;

	typedef std::function<void(
				  const boost::system::error_code& error
			)>
			tconnect_handler;

	typedef std::function<void(
				  const boost::system::error_code& error
				, const size_t bytes_transferred
				, const tmessage* message
			)>
			treceive_handler;

	typedef std::function<void(
				  const boost::system::error_code& error
				, const size_t bytes_transferred
				, const tmessage& message
			)>
			tsend_handler;

	typedef
			std::function<void(const boost::system::error_code&)>
			terror_handler;

	typedef std::function<void(
				  const boost::system::error_code& error
				, const size_t bytes_transferred
			)>
			thandler;


	/***** ***** Constructor, destructor, assignment. ***** *****/

	tconnection() = default;

	virtual ~tconnection();

	tconnection&
	operator=(const tconnection&) = delete;
	tconnection(const tconnection&) = delete;

	tconnection&
	operator=(tconnection&&) = default;
	tconnection(tconnection&&) = default;


	/***** ***** Operators. ***** *****/


	/***** ***** Setters, getters. ***** *****/

	void
	set_protocol(const tprotocol protocol__);

	tprotocol
	get_protocol() const;

	void
	set_error_handler(const terror_handler& error_handler__);

private:

	/***** ***** Members. ***** *****/

	tprotocol protocol_{tprotocol::telnet};

	terror_handler error_handler_{};
};

} // namespace communication

#endif

