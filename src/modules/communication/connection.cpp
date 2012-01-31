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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "communication"

#include "modules/communication/connection.hpp"

#include "lib/exception/validate.tpp"
#include "modules/logging/log.hpp"

namespace communication {

tconnection::~tconnection()
{
	strand_disable();
}

void
tconnection::set_protocol(const tprotocol protocol__)
{
	LOG_T(__PRETTY_FUNCTION__, ": protocol »", protocol__, "«.\n");

	protocol_ = protocol__;
}

tprotocol
tconnection::get_protocol() const
{
	return protocol_;
}


} // namespace communication
