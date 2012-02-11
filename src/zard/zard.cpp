/*
 * Copyright (C) 2011 - 2012 by Mark de Wever <koraq@xs4all.nl>
 * Part of the zard project.
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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "zard"

#include "lib/exception/exception.hpp"
//#include "lib/exception/exit.hpp"
#include "modules/lobby/lobby.hpp"
#include "modules/logging/log.hpp"

#include <stdexcept>
#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
	try {

		LOG_I("Starting.\n");

		lobby::tlobby lobby;

		LOG_I("Finished.\n");

	} catch(const lib::texception& e) {
        LOG_F("Caught '"
                , e.type
                ,"' exception: "
                , e.message, ".\n");

		return EXIT_FAILURE;
	} catch(const std::runtime_error& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
//	} catch(const lib::texit& e) {
//		return e.exit_code;
	}
	return EXIT_SUCCESS;
}

