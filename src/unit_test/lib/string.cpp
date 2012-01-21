/*
 * Copyright (C) 2012 by Mark de Wever <koraq@xs4all.nl>
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

#include "lib/string/concatenate.tpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(lib_string_concatenate)
{
	std::string test = lib::concatenate("abc", 123, 3.141);

	BOOST_CHECK_EQUAL(test, "abc1233.141");
}
