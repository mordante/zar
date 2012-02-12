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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "zard"

#include "zard/options.hpp"

#include "lib/exception/validate.tpp"
#include "lib/exception/exit.hpp"
#include "modules/logging/log.hpp"

static void
print_help()
{
	std::cout <<
"Usage zard [OPTION...] [FILE]\n"
"Starts the zard server programme.\n"
"\n"
"The FILE is the name of the configuration file.\n"
"\n"
"Miscellaneous:\n"
"-f, --foreground  Run the programme in the foreground. If this option is\n"
"                  not supplied the programme will run as daemon.\n"
"-h, --help        Prints this help message and exists.\n";
}

const toptions&
toptions::parse(int argc, char* argv[])
{
	toptions& result = singleton(false);

	bool help{false};

	for(int i = 1; i < argc; ++i) {
		const std::string option{argv[i]};

		if(option == "-h" || option == "--help") {
			help = true;

		} else if(option == "-f" || option == "--foreground") {
			result.foreground = true;

		} else if(result.configuration_file.empty()) {
			result.configuration_file = option;
		} else {
			LOG_F("Option »", option, "« is not recognised.\n");
			print_help();
			throw lib::texit(EXIT_FAILURE);
		}
	}

	if(help) {
		if(argc > 2) {
			LOG_W(
					"The option »-h« and »--help« "
					"can not be combined with other options.\n");
		}
		print_help();
		throw lib::texit(EXIT_SUCCESS);
	}

	return result;
}

const toptions&
toptions::options()
{
	return singleton(true);
}

toptions&
toptions::singleton(const bool is_initialized)
{
	static bool initialized{false};
	static toptions result{};

	VALIDATE(initialized == is_initialized);

	initialized = true;
	return result;
}

