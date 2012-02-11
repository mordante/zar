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

#ifndef MODULES_LOBBY_LOBBY_HPP_INCLUDED
#define MODULES_LOBBY_LOBBY_HPP_INCLUDED

#include "modules/game/game.hpp"
#include "modules/lobby/session.hpp"
#include "modules/lobby/detail/session_reaper.hpp"

#include <boost/asio/io_service.hpp>

#include <thread>
#include <vector>

namespace lobby {

class tlobby final
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tlobby();
	~tlobby() = default;

	tlobby&
	operator=(const tlobby&) = delete;
	tlobby(const tlobby&) = delete;

	tlobby&
	operator=(tlobby&&) = delete;
	tlobby(tlobby&&) = delete;


	/***** ***** Operators. ***** *****/

	void
	user(tsession& session, const std::string& id);

	void
	game_create(tsession& session, const std::string& id);

	std::vector<std::string>
	game_list() const;

	void
	game_list(tsession& session) const;

	/**
	 * Called upon joing but also when leaving a game.
	 * ... not yet implemented
	 */
	void
	join_lobby();

private:

	/***** ***** Members. ***** *****/

	boost::asio::io_service io_service_{};

	boost::asio::ip::tcp::acceptor acceptor_;

	std::vector<game::tgame> games_{};

	std::list<tsession> sessions_{};

	detail::tsession_reaper session_reaper_{io_service_, sessions_};

	std::vector<std::thread> threads_{};

	void
	run();

	void
	stop();

	void
	listen();

	void
	accept_handler(const boost::system::error_code& error);

	void
	receive_handler(
			  tsession& session
			, const boost::system::error_code& error
			, const communication::tmessage* message);

	void
	execute_connected(tsession& session, std::string command);

	void
	execute_lobby(tsession& session, std::string command);

	void
	create_session();
};

} // namespace lobby

#endif
