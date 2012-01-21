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

#ifndef LIB_STRING_CONCATENATE_TPP_INCLUDED
#define LIB_STRING_CONCATENATE_TPP_INCLUDED

#include "lib/string/stream.tpp"

namespace lib {

/**
 * Concatenates a list of variables to a string.
 *
 * It uses the operator<<() for all variables.
 *
 * @param pack                    The parameters to convert to a string.
 *
 * @returns                       The concatenated string.
 */
template<class... Pack>
inline std::string
concatenate(const Pack&... pack)
{
	std::stringstream sstr;
	stream(sstr, pack...);
	return sstr.str();
}

} // namespace lib

#endif
