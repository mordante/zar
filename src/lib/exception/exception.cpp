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

#define ENUM_ENABLE_STREAM_OPERATORS_IMPLEMENTATION
#define ENUM_TYPE lib::texception::ttype
#define ENUM_LIST                                                             \
ENUM(invalid_value,               "E_INVALID_VALUE");                         \
ENUM(busy,                        "E_BUSY");                                  \
ENUM(no_access,                   "E_NO_ACCESS");                             \
ENUM(protocol_error,              "E_PROTOCOL_ERROR");                        \
ENUM(socket_error,                "E_SOCKET_ERROR");                          \
ENUM(not_implemented_yet,         "E_NOT_IMPLEMENTED");                       \
ENUM(internal_failure,            "E_INTERNAL_FAILURE");                      \

#include "lib/exception/exception.hpp"

ENUM_DEFINE_STREAM_OPERATORS(ENUM_TYPE)

namespace lib {

texception::texception(const ttype type__, const std::string& message__)
	: type(type__)
	, message(message__)
{
}

} // namespace lib
