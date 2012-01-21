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

#ifndef LIB_STRING_STREAM_TPP_INCLUDED
#define LIB_STRING_STREAM_TPP_INCLUDED

#include <sstream>

namespace lib {

/**
 * streams a variable.
 *
 * This is a helper function for streaming. It is the terminator for the other
 * @ref stream function.
 *
 * It uses the operator<<().
 *
 * @param ostream                 The output stream to send @p t to.
 * @param t                       The variable to stream.
 *
 * @returns                       The parameter @p ostream.
 */
template<class T>
inline std::ostream&
stream(std::ostream& ostream, const T& t)
{
	ostream << t;
	return ostream;
}

/**
 * streams a list of variables.
 *
 * It uses the operator<<() for all variables.
 *
 * @param ostream                 The output stream to send @p t to.
 * @param t                       A variable to stream.
 * @param pack                    The other parameters to stream.
 *
 * @returns                       The parameter @p ostream.
 */
template<class T, class... Pack>
inline std::ostream&
stream(std::ostream& ostream, const T& t, const Pack&... pack)
{
	ostream << t;
	return stream(ostream, pack...);
}

} // namespace lib

#endif
