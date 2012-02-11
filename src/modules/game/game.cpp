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

#include "modules/game/game.hpp"

#include "lib/exception/validate.tpp"
#include "lib/string/concatenate.tpp"
#include "modules/logging/log.hpp"

namespace game {

tgame::tgame(lobby::tsession& session, const std::string& id__)
	: id_(id__)
{
	/**
	 * @todo @c emplace() doesn't seem to exist. Once there also look
	 * whether the player can drop its move capabilities.
	 */
	players_.insert(std::make_pair(
			  &session
			, detail::tplayer(session.get_id(), detail::tplayer::trole::gm)));

	session.set_mode(lobby::tsession::tmode::creating_game);

	session.send(lib::concatenate("OK\nCreated game '", id__, "'.\n"));

	/**
	 * @todo We reset the receive_handler but never restore the original
	 * handler.
	 */
	session.set_receive_handler(std::bind(
			  &tgame::receive_handler
			, this
			, std::ref(session)
			, std::placeholders::_1
			, std::placeholders::_3));
}

const std::string&
tgame::id() const
{
	return id_;
}

void
tgame::receive_handler(
		  lobby::tsession& session
		, const boost::system::error_code& error
		, const communication::tmessage* message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "«.\n");

	if(error) {
		return;
	}

	VALIDATE(message);

	switch(session.get_mode()) {
		case lobby::tsession::tmode::connected :
			/* FALL DOWN */

		case lobby::tsession::tmode::lobby :
			/*FAIL*/; throw 0;

		case lobby::tsession::tmode::creating_game :
			execute_creating_game(session, message->contents());
	}
}

void
tgame::execute_creating_game(lobby::tsession& session, std::string command)
{
	LOG_D("Execute »", command, "«.\n");

	try {
		if(command.empty()) {
			/* Do nothing. */
		} else if(command == "help") {
			session.send("OK\nStill no help available.\n");
		} else {
			session.send("EINVAL\nUnknown command.\n");
		}
	} catch(...) {
		session.send("ERROR\n");
	}
}

} // namespace game
