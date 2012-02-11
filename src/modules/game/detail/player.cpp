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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "game"

#define ENUM_ENABLE_STREAM_OPERATORS_IMPLEMENTATION
#define ENUM_TYPE ::game::detail::tplayer::trole
#define ENUM_LIST                                                             \
ENUM(gm,                          "gm");                                      \
ENUM(player,                      "player");                                  \

#include "modules/game/detail/player.hpp"

#include "lib/exception/validate.tpp"
#include "modules/logging/log.hpp"

ENUM_DEFINE_STREAM_OPERATORS(ENUM_TYPE)

namespace game {

namespace detail {

tplayer::tplayer(const std::string& name__, const trole role__)
	: name_(name__)
	, role_(role__)
{
}

void
tplayer::set_name(const std::string& name__)
{
	LOG_T(__PRETTY_FUNCTION__, ": name »", name__, "«.\n");

	VALIDATE(!name__.empty());

	name_ = name__;
}

std::string
tplayer::get_name() const
{
	return name_;
}

void
tplayer::set_role(const trole role__)
{
	LOG_T(__PRETTY_FUNCTION__, ": name »", role__, "«.\n");

	role_ = role__;
}

tplayer::trole
tplayer::get_role() const
{
	return role_;
}

} // namespace detail

} // namespace game
