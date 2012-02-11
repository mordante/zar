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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "client"

#include "czar/application.hpp"
#include "lib/exception/validate.tpp"
//#include "lib/exception/exit.hpp"
#include "modules/logging/log.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <thread>

namespace logging {

class tfile final
{
public:

	tfile(const std::string& filename);
	~tfile() = default;

	tfile&
	operator=(const tfile&) = delete;
	tfile(const tfile&) = delete;

	tfile&
	operator=(tfile&&) = delete;
	tfile(tfile&&) = delete;


	std::ofstream*
	ostream();

private:

	std::ofstream ostream_;
};

tfile::tfile(const std::string& filename)
	: ostream_(filename.c_str(), std::ios_base::app | std::ios_base::binary)
{
	if(!ostream_.is_open()) {
		throw lib::texception(lib::texception::ttype::no_access
				, lib::concatenate(
					  "Couldn't open »"
					, filename
					, "« for writing"));
	}
}

std::ofstream*
tfile::ostream()
{
	return ostream_.is_open() ? &ostream_ : nullptr;
}

} // namespace logging

static std::string
log_level_to_string(const logging::tlevel level)
{
	switch(level) {
		case logging::tlevel::trace       : return "TRACE";
		case logging::tlevel::debug       : return "DEBUG";
		case logging::tlevel::information : return "INFO ";
		case logging::tlevel::warning     : return "WARN ";
		case logging::tlevel::error       : return "ERROR";
		case logging::tlevel::fatal       : return "FATAL";
	}

	ENUM_FAIL_RANGE(level);
}

void
module_logger_multi_threaded_pre_process_function(
		  const std::string& module
		, const logging::tlevel level
		, std::string& message);
void
module_logger_multi_threaded_pre_process_function(
		  const std::string& module
		, const logging::tlevel level
		, std::string& message)
{

	static std::atomic<unsigned> id;
	static std::map<std::thread::id, unsigned> threads;

	std::thread::id this_thread_id = std::this_thread::get_id();

	if(threads.count(this_thread_id) == 0) {
		threads[this_thread_id] = id++;
	}

	message = lib::concatenate(
			  boost::posix_time::to_iso_string(
				  boost::posix_time::microsec_clock::universal_time())
			, " {"
			, threads[this_thread_id]
			, "}"
			, " ["
			, log_level_to_string(level)
			, "] "
			, module
			, ": "
			, message);
}

void
module_logger_pre_process_function(
		  const std::string& module
		, const logging::tlevel level
		, std::string& message);
void
module_logger_pre_process_function(
		  const std::string& module
		, const logging::tlevel level
		, std::string& message)
{
	message = lib::concatenate(
			  boost::posix_time::to_iso_string(
				  boost::posix_time::microsec_clock::universal_time())
			, " ["
			, log_level_to_string(level)
			, "] "
			, module
			, ": "
			, message);
}

int main(int /*argc*/, char* /*argv*/[])
{
	try {

		logging::tfile logfile("client.log");
		logging::module::module_logger().set_stream_function(std::bind(
				  &logging::tfile::ostream
				, &logfile));

		logging::module::module_logger().set_pre_process_function(std::bind(
				  module_logger_multi_threaded_pre_process_function
				, std::placeholders::_1
				, std::placeholders::_2
				, std::placeholders::_3));

		LOG_I("Starting.\n");

//		options.parse(argc, argv);

		tapplication application;

		LOG_I("Finished.\n");


	} catch(const lib::texception& e) {
		std::cerr << e.message << ".\n";
		return EXIT_FAILURE;
	} catch(const std::runtime_error& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
//	} catch(const lib::texit& e) {
//		return e.exit_code;
	}
	return EXIT_SUCCESS;
}

