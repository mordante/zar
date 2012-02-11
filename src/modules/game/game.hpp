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

#ifndef MODULES_GAME_GAME_HPP_INCLUDED
#define MODULES_GAME_GAME_HPP_INCLUDED

#include "modules/game/detail/player.hpp"
#include "modules/lobby/session.hpp"

#include <map>

namespace game {

class tgame final
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tgame(lobby::tsession& session, const std::string& id__);

	~tgame() = default;

	tgame&
	operator=(const tgame&) = delete;
	tgame(const tgame&) = delete;

	tgame&
	operator=(tgame&&) = default;
	tgame(tgame&&) = default;


	/***** ***** Setters, getters. ***** *****/

	const std::string&
	id() const;

private:

	/***** ***** Members. ***** *****/

	/** The id of the game. */
	std::string id_;

	/*
	 * Players and GM.
	 *
	 * Pointers are owned by the lobby.
	 */
	std::map<lobby::tsession*, detail::tplayer> players_{};

	void
	receive_handler(
			  lobby::tsession& session
			, const boost::system::error_code& error
			, const communication::tmessage* message);

	/**
	 * Execution handler when in the @ref creating_game mode.
	 *
	 * @param session             The session sending the command.
	 * @param command             The command to be executed.
	 */
	void
	execute_creating_game(lobby::tsession& session, std::string command);
};

} // namespace game

#endif
