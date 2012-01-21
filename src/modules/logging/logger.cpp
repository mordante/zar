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

#include "modules/logging/logger.hpp"

#include "lib/exception/validate.tpp"

namespace logging {

tlogger::~tlogger()
{
	set_sync_mode();
}

bool
tlogger::is_active(const tlevel level) const
{
	return static_cast<int>(level) >= static_cast<int>(threshold_level_);
}

void
tlogger::set_async_mode(boost::asio::io_service& io_service)
{
	set_sync_mode();
	strand_ = new boost::asio::io_service::strand(io_service);
}

void
tlogger::set_sync_mode()
{
	delete strand_;
	strand_ = nullptr;
}

static void
do_log(const tlevel level
		, std::string message
		, tlogger::tstream_function stream_function
		, tlogger::tpost_process_function post_process_function)
{
	if(!stream_function) {
		return;
	}

	std::ostream* ostream = stream_function(level);

	if(!ostream) {
		return;
	}

	*ostream << message;

	if(post_process_function) {
		post_process_function();
	}
}
void
tlogger::log(const tlevel level, std::string message)
{
	if(!is_active(level)) {
		return;
	}

	if(pre_process_function_) {
		pre_process_function_(level, message);
	}

	if(strand_) {
		strand_->post(std::bind(
				  do_log
				, level
				, std::move(message)
				, stream_function_
				, post_process_function_));
	} else {
		do_log(level
				, std::move(message)
				, stream_function_
				, post_process_function_);
	}
}

void
tlogger::set_threshold_level(const tlevel threshold_level__)
{
	threshold_level_ = threshold_level__;
}

tlevel
tlogger::get_threshold_level() const
{
	return threshold_level_;
}

boost::asio::io_service::strand*
tlogger::strand()
{
	return strand_;
}

void
tlogger::set_pre_process_function(const tpre_process_function& function)
{
	pre_process_function_ = function;
}

void
tlogger::set_stream_function(const tstream_function& function)
{
	stream_function_ = function;
}

void
tlogger::set_post_process_function(const tpost_process_function& function)
{
	post_process_function_ = function;
}

std::ostream*
tlogger::basic_stream_function(const tlevel level)
{
	switch(level) {
		case tlevel::trace:
			/* FALL DOWN */
		case tlevel::debug:
			/* FALL DOWN */
		case tlevel::information:
			/* FALL DOWN */
		case tlevel::warning:
			return &std::cout;
			break;

		case tlevel::error:
			/* FALL DOWN */
		case tlevel::fatal:
			return &std::cerr;
			break;
	}

	ENUM_FAIL_RANGE(level);
}

} // namespace logging
