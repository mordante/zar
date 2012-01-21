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

#include <algorithm>

BOOST_AUTO_TEST_CASE(lib_string_concatenate)
{
	std::string test = lib::concatenate("abc", 123, 3.141);

	BOOST_CHECK_EQUAL(test, "abc1233.141");
}

enum class tunit_test
{
	  enum1 = 0
	, enum2
};

#define ENUM_ENABLE_STREAM_OPERATORS_IMPLEMENTATION
#define ENUM_TYPE tunit_test
#define ENUM_LIST                                                             \
ENUM(enum1,                       "Enum 1");                                  \
ENUM(enum2,                       "Enum 2");                                  \

#include "lib/string/enumerate.tpp"

ENUM_DECLARE_STREAM_OPERATORS(tunit_test)

ENUM_DEFINE_STREAM_OPERATORS(ENUM_TYPE)

BOOST_AUTO_TEST_CASE(lib_string_enum)
{
	/*** Input ***/

	{
		std::string str("Enum 1");
		tunit_test estr;

		std::stringstream sstr("Enum 2");
		tunit_test esstr;

		str >> estr;
		sstr >> esstr;

		BOOST_CHECK_EQUAL(estr, tunit_test::enum1);
		BOOST_CHECK_EQUAL(esstr, tunit_test::enum2);
	}

	{
		std::string str("Not an enum value");
		tunit_test estr;

		std::stringstream sstr("Not an enum value");
		tunit_test esstr;

		BOOST_CHECK_EXCEPTION(
				  str >> estr;
				, lib::texception
				, [](const lib::texception& exception)
					{
						return exception.type
								== lib::texception::ttype::invalid_value;
					});

		BOOST_CHECK_EXCEPTION(
				  sstr >> esstr;
				, lib::texception
				, [](const lib::texception& exception)
					{
						return exception.type
								== lib::texception::ttype::invalid_value;
					});
	}


	/*** Output ***/

	{
		std::string str;
		std::stringstream sstr;

		str << tunit_test::enum1;
		sstr << tunit_test::enum2;

		BOOST_CHECK_EQUAL(str, "Enum 1");
		BOOST_CHECK_EQUAL(sstr.str(), "Enum 2");

	}

	{
		std::string str;
		std::stringstream sstr;

		BOOST_CHECK_EXCEPTION(
				  str << static_cast<tunit_test>(-1)
				, lib::texception
				, [](const lib::texception& exception)
					{
						return exception.type
								== lib::texception::ttype::invalid_value;
					});

		BOOST_CHECK_EXCEPTION(
				  sstr << static_cast<tunit_test>(-1)
				, lib::texception
				, [](const lib::texception& exception)
					{
						return exception.type
								== lib::texception::ttype::invalid_value;
					});
	}
}
