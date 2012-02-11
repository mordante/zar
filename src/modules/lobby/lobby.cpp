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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "lobby"

#include "modules/lobby/lobby.hpp"

#include "lib/exception/validate.tpp"
#include "modules/logging/log.hpp"

namespace lobby {

tlobby::tlobby()
	  /*
	   * Port number and protocol should be a setting.
	   * Maybe even dual-stack, both ipv4 and ipv6 acceptor.
	   */
	: acceptor_(
		  io_service_
		, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 2048))
{
	run();
}

void
tlobby::user(tsession& session, const std::string& id)
{
	LOG_T(__PRETTY_FUNCTION__, ": id »", id, "«.\n");

	for(const tsession& s : sessions_) {
		if(s.get_id() == id) {
			throw lib::texception(
					  lib::texception::ttype::busy
					, lib::concatenate(
						  "A user with id »"
						, id
						, "« already logged in"));
		}
	}

	session.set_id(id);
	session.set_mode(tsession::tmode::lobby);
	session.send("OK\n");
}

void
tlobby::game_create(tsession& session, const std::string& id)
{
	LOG_T(__PRETTY_FUNCTION__, ": id »", id, "«.\n");

	for(const game::tgame& game : games_) {
		if(game.id() == id) {
			throw lib::texception(
					  lib::texception::ttype::busy
					, lib::concatenate(
						  "A game with id »"
						, id
						, "« is already created"));
		}
	}

	games_.push_back(game::tgame(session, id));
}

std::vector<std::string>
tlobby::game_list() const
{
	std::vector<std::string> result;
	for(const game::tgame& game : games_) {
		result.push_back(game.id());
	}
	return result;
}

void
tlobby::game_list(tsession& session) const
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	std::string result = "OK\n";
	for(const game::tgame& game : games_) {
		result += game.id();
		result += '\n';
	}
	session.send(result);
}

static void
loop(boost::asio::io_service& io_service)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	while(true) {
		try {
			io_service.run();
		} catch(...) {
		}
	}
}

void
tlobby::run()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	logging::module::set_async_mode(io_service_);

	create_session();
	listen();

	session_reaper_.run();

	/* Number of threads hard-coded. */
	for(int i = 0; i < 2; ++i) {
		threads_.push_back(std::thread(loop, std::ref(io_service_)));
	}

	loop(io_service_);
}

void
tlobby::stop()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	logging::module::set_sync_mode();
	io_service_.stop();

	for(std::thread& thread : threads_) {
		thread.join();
	}
}

void
tlobby::listen()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	sessions_.back().accept(acceptor_);
}

void
tlobby::accept_handler(const boost::system::error_code& error)
{
	LOG_T(__PRETTY_FUNCTION__, ": error »", error.message(), "«.\n");

	if(!error) {
		LOG_D("Session: starting.\n");
		sessions_.back().set_status(tsession::tstatus::connected);
		sessions_.back().send("Zard\n1");
		sessions_.back().receive();

		create_session();
	}

	listen();
}

void
tlobby::receive_handler(
		  tsession& session
		, const boost::system::error_code& error
		, const communication::tmessage* message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« message.id »", message ? message->id() : 0
			, "« message.data »", message ? message->contents() : "NULL"
			, "«.\n");

	if(error) {
		return;
	}

	VALIDATE(message);

	switch(session.get_mode()) {
		case tsession::tmode::connected :
			execute_connected(session, message->contents());
			break;

		case tsession::tmode::lobby :
			execute_lobby(session, message->contents());
			break;

		case tsession::tmode::creating_game :
			/*FAIL*/;
	}
}

void
tlobby::execute_connected(tsession& session, std::string command)
{
	LOG_D("Connected: Execute '", command, "'\n");

	static const std::string cmd_user = "user ";

	try {
		if(command.empty()) {
			/* Do nothing. */
		} else if(command == "help") {
			session.send("OK\nNo help available.\n");
		} else if(command.substr(0, cmd_user.length()) == cmd_user) {
			user(session, command.substr(cmd_user.length()));
		} else {
			session.send("EINVAL\nUnknown command.\n");
		}
	} catch(...) {
		session.send("ERROR\n");
	}
}

void
tlobby::execute_lobby(tsession& session, std::string command)
{
	LOG_D("Lobby: Execute '", command, "'\n");

	static const std::string cmd_game_create = "game create ";
	static const std::string cmd_game_list = "game list";


	try {
		if(command.empty()) {
			/* Do nothing. */
		} else if(command == "help") {
			session.send("OK\nNo help available.\n");
		} else if(command.substr(0, cmd_game_create.length())
				== cmd_game_create) {
			game_create(session, command.substr(cmd_game_create.length()));
		} else if(command == cmd_game_list) {
			game_list(session);
		} else {
			session.send("EINVAL\nUnknown command.\n");
		}
	} catch(...) {
		session.send("ERROR\n");
	}
}

void
tlobby::create_session()
{
	sessions_.emplace_back(io_service_);

	sessions_.back().set_accept_handler(std::bind(
			  &tlobby::accept_handler
			, this
			, std::placeholders::_1));

	sessions_.back().set_receive_handler(std::bind(
			  &tlobby::receive_handler
			, this
			, std::ref(sessions_.back())
			, std::placeholders::_1
			, std::placeholders::_3));
}

} // namespace lobby
