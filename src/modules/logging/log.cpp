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

#include "modules/logging/log.hpp"

namespace logging {

namespace basic {

tlogger&
logger()
{
	static tlogger result;
	return result;
}

bool
is_active(const tlevel level)
{
	return logger().is_active(level);
}

void
set_async_mode(boost::asio::io_service& io_service)
{
	logger().set_async_mode(io_service);
}

void
set_sync_mode()
{
	logger().set_sync_mode();
}

void
log(const tlevel level, std::string message)
{
	logger().log(level, std::move(message));
}

void
set_threshold_level(const tlevel threshold_level)
{
	logger().set_threshold_level(threshold_level);
}

} // namespace basic

namespace module {

tmodule_logger&
module_logger()
{
	static tmodule_logger result;
	return result;
}

bool
is_active(const std::string& module, const tlevel level)
{
	return module_logger().is_active(module, level);
}

void
set_async_mode(boost::asio::io_service& io_service)
{
	module_logger().set_async_mode(io_service);
}

void
set_sync_mode()
{
	module_logger().set_sync_mode();
}

void
log(const std::string& module
		, const tlevel level
		, std::string message)
{
	module_logger().log(module, level, message);
}


void
set_threshold_level(const tlevel threshold_level)
{
	module_logger().set_threshold_level(threshold_level);
}

void
set_threshold_level(
		  const std::string& module
		, const tlevel threshold_level)
{
	module_logger().set_threshold_level(module, threshold_level);
}

tregisterer::tregisterer(const std::string& module)
{
	module_logger().register_module(module);
}

} // namespace module
} // namespace logging
