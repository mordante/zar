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

#include "modules/logging/module_logger.hpp"

#include "lib/exception/validate.tpp"

namespace logging {

tmodule_logger::~tmodule_logger()
{
	set_sync_mode();
}

void
tmodule_logger::register_module(
			  const std::string& module
			, const tlevel threshold_level)
{
	modules_.insert(std::make_pair(module, threshold_level));
}

bool
tmodule_logger::is_active(const std::string& module, const tlevel level) const
{
	if(modules_.count(module) == 0) {
		return false;
	}

	return static_cast<int>(level) >= static_cast<int>(modules_.at(module));
}

void
tmodule_logger::set_async_mode(boost::asio::io_service& io_service)
{
	set_sync_mode();
	strand_ = new boost::asio::io_service::strand(io_service);
}

void
tmodule_logger::set_sync_mode()
{
	delete strand_;
	strand_ = nullptr;
}

static void
do_log(const std::string& module
		, const tlevel level
		, std::string message
		, tmodule_logger::tstream_function stream_function
		, tmodule_logger::tpost_process_function post_process_function)
{
	if(!stream_function) {
		return;
	}

	std::ostream* ostream = stream_function(module, level);

	if(!ostream) {
		return;
	}

	*ostream << message;

	if(post_process_function) {
		post_process_function(std::cref(module));
	}
}

void
tmodule_logger::log(
		  const std::string& module
		, const tlevel level
		, std::string message)
{
	if(!is_active(module, level)) {
		return;
	}

	if(pre_process_function_) {
		pre_process_function_(std::cref(module), level, message);
	}

	if(strand_) {
		strand_->post(std::bind(
				  do_log
				, std::cref(module)
				, level
				, std::move(message)
				, stream_function_
				, post_process_function_));
	} else {
		do_log(module
				, level
				, std::move(message)
				, stream_function_
				, post_process_function_);
	}
}

void
tmodule_logger::set_threshold_level(const tlevel threshold_level)
{
	for(auto& module : modules_) {
		module.second = threshold_level;
	}
}

void
tmodule_logger::set_threshold_level(
			  const std::string& module
			, const tlevel threshold_level)
{
	if(modules_.count(module) == 0) {
		throw lib::texception(
				  lib::texception::ttype::invalid_value
				, lib::concatenate(
					  "Logging module '"
					, module
					, "' is not registered"));
	}

	modules_[module] = threshold_level;
}

tlevel
tmodule_logger::get_threshold_level(const std::string& module) const
{
	if(modules_.count(module) == 0) {
		throw lib::texception(
				  lib::texception::ttype::invalid_value
				, lib::concatenate(
					  "Logging module '"
					, module
					, "' is not registered"));
	}

	return modules_.at(module);
}

boost::asio::io_service::strand*
tmodule_logger::strand()
{
	return strand_;
}

void
tmodule_logger::set_pre_process_function(const tpre_process_function& function)
{
	pre_process_function_ = function;
}

void
tmodule_logger::set_stream_function(const tstream_function& function)
{
	stream_function_ = function;
}

void
tmodule_logger::set_post_process_function(const tpost_process_function& function)
{
	post_process_function_ = function;
}

std::ostream*
tmodule_logger::basic_stream_function(const tlevel level)
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
