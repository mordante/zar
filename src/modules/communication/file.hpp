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

#ifndef MODULES_COMMUNICATION_FILE_HPP_INCLUDED
#define MODULES_COMMUNICATION_FILE_HPP_INCLUDED

#include "modules/communication/receiver.hpp"
#include "modules/communication/sender.hpp"

namespace communication {

class tfile final
	: public treceiver_file
	, public tsender_file
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tfile(boost::asio::io_service& io_service, const int fd);

	virtual ~tfile();

	tfile&
	operator=(const tfile&) = delete;
	tfile(const tfile&) = delete;

	tfile&
	operator=(tfile&&) = default;
	tfile(tfile&&) = default;
};

} // namespace communication

#endif

