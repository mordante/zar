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

#include "zard/configuration.hpp"

#include "lib/exception/exception.hpp"
#include "lib/exception/exit.hpp"
#include "modules/logging/log.hpp"
#include "zard/options.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <thread>

/**
 * Helper conversion structure.
 *
 * Allows @ref boost::property_tree::ptree to use a @ref logging::tlevel as
 * variable.
 */
struct tlog_level_convertor
{
	logging::tlevel
	get_value(const std::string& value) const
	{
		logging::tlevel result;
		value >> result;
		return result;
	}
};

const tconfiguration&
tconfiguration::configuration()
{
	static const tconfiguration& result = create();
	return result;
}

static std::string
get_configuration_filename()
{
	const toptions& options = toptions::options();

	if(!options.configuration_file.empty()) {
		return options.configuration_file;
	}

	return "/etc/zardrc";
}

tconfiguration&
tconfiguration::create()
{
	static tconfiguration result;

	const std::string configuration_filename{get_configuration_filename()};

	try {

		boost::property_tree::ptree ini;

		boost::property_tree::ini_parser::read_ini(configuration_filename, ini);

		result.threads = ini.get("threads", result.threads);
		result.port = ini.get("port", result.port);
		result.reap_interval = ini.get("reap_interval", result.reap_interval);

		logging::tlevel log_level = ini.get(
				  "log_level/global"
				, logging::tlevel::trace
				, tlog_level_convertor());

		logging::module::set_threshold_level(log_level);

		/**
		 * @todo Add a log level for every module.
		 * E.g. log_level/zard, log_level/lobby, etc. etc..
		 */

	} catch(const boost::property_tree::ini_parser::ini_parser_error& e) {
		/* If cannot open the file just warn and proceed. */
		if(e.line() == 0) {
			LOG_W(
					  "Failed to open configuration file »"
					, e.filename()
					, "« message »", e.message()
					, "«.\n");

		} else {
			throw lib::texception(
					  lib::texception::ttype::invalid_value
					, lib::concatenate(
						  "Ini parser error: file »"
						, e.filename(), ':', e.line()
						, "« message »", e.message()
						, "«"));
		}
	}

	result.validate();

	return result;
}

void
tconfiguration::validate()
{
	if(threads == 0) {
		threads = std::thread::hardware_concurrency();
		LOG_I(
				  "Automatic hardware detections set the number of threads to »"
				, threads
				, "«.\n");
		if(threads == 0) {
			LOG_W(
					  "Automatic hardware detection for the number of threads "
					  "failed, set to »1«.\n");
			threads = 1;
		}
	}

	if(port == 0) {
		throw lib::texception(
				  lib::texception::ttype::invalid_value
				, "Port »0« is not a valid port number");
	}

	if(reap_interval == 0) {
		LOG_W("Reap interval of »0« is invalid, set to »1«.\n");
		reap_interval = 1;
	}
}
