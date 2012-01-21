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

/**
 * @file
 * Contains the wrappers for the logger classes.
 */

#ifndef MODULES_LOGGING_LOG_HPP_INCLUDED
#define MODULES_LOGGING_LOG_HPP_INCLUDED

#include "modules/logging/logger.hpp"
#include "modules/logging/module_logger.hpp"

namespace logging {
namespace basic {

tlogger&
logger();

bool
is_active(const tlevel level);

void
set_async_mode(boost::asio::io_service& io_service);

void
set_sync_mode();

void
log(const tlevel level, std::string message);

template<class... Pack>
inline void
log(const tlevel level, const Pack&... pack)
{
	if(!is_active(level)) {
		return;
	}

	log(level, lib::concatenate(pack...));
}

void
set_threshold_level(const tlevel threshold_level);

#ifdef LOGGER_DEFINE_BASIC_LOGGER_MACROS /* NOT TESTED */
#ifdef LOGGER_DEFINE_MODULE_LOGGER_MACROS
#error "Only one set of logger macros can be defined."
#endif

#define LOG_T(...)                                                           \
	::logging::basic::log(                                                   \
			  ::logging::tlevel::trace                                       \
			, __VA_ARGS__)                                                   \

#define LOG_D(...)                                                           \
	::logging::basic::log(                                                   \
			  ::logging::tlevel::debug                                       \
			, __VA_ARGS__)                                                   \

#define LOG_I(...)                                                           \
	::logging::basic::log(                                                   \
			  ::logging::tlevel::information                                 \
			, __VA_ARGS__)                                                   \

#define LOG_W(...)                                                           \
	::logging::basic::log(                                                   \
			  ::logging::tlevel::information                                 \
			, __VA_ARGS__)                                                   \

#define LOG_E(...)                                                           \
	::logging::basic::log(                                                   \
			  ::logging::tlevel::error                                       \
			, __VA_ARGS__)                                                   \

#define LOG_F(...)                                                           \
	::logging::basic::log(                                                   \
			  ::logging::tlevel::fatal                                       \
			, __VA_ARGS__)                                                   \

#endif

} // namespace basic

namespace module {

tmodule_logger&
module_logger();

bool
is_active(const std::string& module, const tlevel level);

void
set_async_mode(boost::asio::io_service& io_service);

void
set_sync_mode();

void
log(const std::string& module
		, const tlevel level
		, std::string message);

template<class... Pack>
inline void
log(const std::string& module
		, const tlevel level
		, const Pack&... pack)
{
	if(!is_active(module, level)) {
		return;
	}

	log(module, level, lib::concatenate(pack...));
}

void
set_threshold_level(const tlevel threshold_level);

void
set_threshold_level(
		  const std::string& module
		, const tlevel threshold_level);

struct tregisterer
{
	explicit tregisterer(const std::string& module);
	tregisterer() = default;

	tregisterer&
	operator=(const tregisterer&) = delete;
	tregisterer(const tregisterer&) = delete;

	tregisterer&
	operator=(tregisterer&&) = delete;
	tregisterer(tregisterer&&) = delete;
};

} // namespace module
} // namespace logging

#ifdef LOGGER_DEFINE_MODULE_LOGGER_MACROS
#ifdef LOGGER_DEFINE_BASIC_LOGGER_MACROS
#error "Only one set of logger macros can be defined."
#endif

#define LOGGER_REGISTER_MODULE(MODULE)                                       \
	namespace {                                                              \
		::logging::module::tregisterer registerer(MODULE);                   \
	}                                                                        \

LOGGER_REGISTER_MODULE(LOGGER_DEFINE_MODULE_LOGGER_MACROS)
#undef LOGGER_REGISTER_MODULE

#define LOG_T(...)                                                           \
	::logging::module::log(                                                  \
			  LOGGER_DEFINE_MODULE_LOGGER_MACROS                             \
			, ::logging::tlevel::trace                                       \
			, __VA_ARGS__)                                                   \

#define LOG_D(...)                                                           \
	::logging::module::log(                                                  \
			  LOGGER_DEFINE_MODULE_LOGGER_MACROS                             \
			, ::logging::tlevel::debug                                       \
			, __VA_ARGS__)                                                   \

#define LOG_I(...)                                                           \
	::logging::module::log(                                                  \
			  LOGGER_DEFINE_MODULE_LOGGER_MACROS                             \
			, ::logging::tlevel::information                                 \
			, __VA_ARGS__)                                                   \

#define LOG_W(...)                                                           \
	::logging::module::log(                                                  \
			  LOGGER_DEFINE_MODULE_LOGGER_MACROS                             \
			, ::logging::tlevel::information                                 \
			, __VA_ARGS__)                                                   \

#define LOG_E(...)                                                           \
	::logging::module::log(                                                  \
			  LOGGER_DEFINE_MODULE_LOGGER_MACROS                             \
			, ::logging::tlevel::error                                       \
			, __VA_ARGS__)                                                   \

#define LOG_F(...)                                                           \
	::logging::module::log(                                                  \
			  LOGGER_DEFINE_MODULE_LOGGER_MACROS                             \
			, ::logging::tlevel::fatal                                       \
			, __VA_ARGS__)                                                   \

#endif
#endif
