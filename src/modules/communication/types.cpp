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

#define ENUM_ENABLE_STREAM_OPERATORS_IMPLEMENTATION
#define ENUM_TYPE ::communication::tprotocol
#define ENUM_LIST                                                             \
ENUM(direct,                      "direct");                                  \
ENUM(line,                        "line");                                    \
ENUM(telnet,                      "telnet");                                  \
ENUM(basic,                       "basic");                                   \
ENUM(compressed,                  "compressed");                              \

#include "modules/communication/types.hpp"

ENUM_DEFINE_STREAM_OPERATORS(ENUM_TYPE)

namespace communication {

} // namespace communication
