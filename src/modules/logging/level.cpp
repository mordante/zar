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
#define ENUM_TYPE logging::tlevel
#define ENUM_LIST \
ENUM(trace,                       "trace");                                   \
ENUM(debug,                       "debug");                                   \
ENUM(information,                 "information");                             \
ENUM(warning,                     "warning");                                 \
ENUM(error,                       "error");                                   \
ENUM(fatal,                       "fatal");                                   \

#include "modules/logging/level.hpp"

ENUM_DEFINE_STREAM_OPERATORS(ENUM_TYPE)

namespace logging {

} // namespace logging
