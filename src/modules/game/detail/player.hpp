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

#ifndef MODULES_GAME_DETAIL_PLAYER_HPP_INCLUDED
#define MODULES_GAME_DETAIL_PLAYER_HPP_INCLUDED

#include <string>

namespace game {

namespace detail {

class tplayer final
{
public:

	/***** ***** Types. *****/

	/** The roles a player can perform. */
	enum class trole
	{
		  gm
		, player
	};


	/***** ***** Constructor, destructor, assignment. ***** *****/

	tplayer(const std::string& name__, const trole role__);
	~tplayer() = default;

	tplayer&
	operator=(const tplayer&) = delete;
	tplayer(const tplayer&) = delete;

	tplayer&
	operator=(tplayer&&) = default;
	tplayer(tplayer&&) = default;


	/***** ***** Setters, getters. ***** *****/

	void
	set_name(const std::string& name__);

	std::string
	get_name() const;

	void
	set_role(const trole role__);

	trole
	get_role() const;

private:

	/***** ***** Members. ***** *****/

	/** The name of the player. */
	std::string name_;

	/** The role of the player. */
	trole role_;
};

} // namespace detail

} // namespace game

#include "lib/string/enumerate.tpp"

ENUM_DECLARE_STREAM_OPERATORS(::game::detail::tplayer::trole)

#endif
