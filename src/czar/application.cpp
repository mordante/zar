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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "czar"

#include "czar/application.hpp"

#include "lib/exception/validate.tpp"
//#include "lib/exception/exit.hpp"
#include "modules/logging/log.hpp"
#include "lib/string/concatenate.tpp"

#include <csignal>

/**
 *
 * TODO need a simple logger class, no levels just logging
 * this class can then be used to log the data over the communication protocol.
 */

tapplication::tapplication()
{
	input_.set_receive_handler(std::bind(
			  &tapplication::execute
			, this
			, std::placeholders::_1
			, std::placeholders::_3));

	input_.set_protocol(communication::tprotocol::line);
	input_.strand_enable(io_service_);

	output_.set_protocol(communication::tprotocol::line);
	output_.strand_enable(io_service_);

	socket_.set_connect_handler(std::bind(
			  &tapplication::asio_callback_socket_connect
			, this
			, std::placeholders::_1));

	socket_.set_send_handler(std::bind(
			  &tapplication::asio_callback_socket_send
			, this
			, std::placeholders::_1
			, std::placeholders::_3));

	socket_.set_receive_handler(std::bind(
			  &tapplication::asio_callback_socket_receive
			, this
			, std::placeholders::_1
			, std::placeholders::_3));

	socket_.strand_enable(io_service_);

	run();
}

void
tapplication::run()
{
	signals_.async_wait(std::bind(&tapplication::asio_callback_signal, this));
	logging::module::set_async_mode(io_service_);

	input_.receive();

	for(int i = 0; i < 4; ++i) {
		threads_.push_back(std::thread(&tapplication::loop, this));
	}

	loop();
}

void
tapplication::loop()
{
	do {
		try {
			io_service_.run();

		} catch(const lib::texception& e) {
			LOG_E("Main loop error: message »",  e.message, "«.\n");
		} catch(const std::runtime_error& e) {
			LOG_E("Main loop error: message »",  e.what(), "«.\n");
//		} catch(const lib::texit& e) {
//			return e.exit_code;
		} catch(...) {
			LOG_E("Main loop unexpected error.\n");
		}
	} while(!io_service_.stopped());
}

void
tapplication::execute(
		  const boost::system::error_code& error
		, const communication::tmessage* message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« data »", message ? message->contents() : "NULL"
			, "«.\n");

	if(error) {
		return;
	}

	VALIDATE(message);

	const std::string& command = message->contents();

	if(command.empty()) {
	} else if(command[0] == '/') {
		execute_client(command.substr(1));
	} else {
		execute_server(command);
	}
}

std::string
tapplication::execute_client(const std::string& command)
{
	LOG_T(__PRETTY_FUNCTION__, ": command »", command, "«.\n");

	static const std::string begin_cmd = "begin";
	static const std::string end_cmd = "end";
	static const std::string connect_cmd = "connect ";

	if(command == begin_cmd) {
		if(buffering_mode_) {
			throw lib::texception(
					  lib::texception::ttype::busy
					, "Already buffering input");
		}
		buffering_mode_ = true;

	} else if(command == end_cmd) {
		if(!buffering_mode_) {
			throw lib::texception(
				  lib::texception::ttype::protocol_error
				, "Not in buffering mode");
		}
		buffering_mode_ = false;
		/*
		 * If not empty need to strip the last new line
		 */
		std::string cmd;
		std::swap(command_buffer_, cmd);
		return execute_server(cmd);

	} else if(command.substr(0, connect_cmd.size()) == connect_cmd) {
		if(connected_) {
			throw lib::texception(
					  lib::texception::ttype::busy
					, "Already connected");
		}
		connect(command.substr(connect_cmd.size()));
//		VALIDATE(connected_);
	} else {
		throw lib::texception(
			  lib::texception::ttype::invalid_value
			, lib::concatenate("Commend »", command, "« is not supported"));
	}

	return "";
}

std::string
tapplication::execute_server(const std::string& command)
{
	LOG_T(__PRETTY_FUNCTION__, ": command »", command, "«.\n");

	if(!connected_) {
		throw lib::texception(
				  lib::texception::ttype::socket_error
				, "Not connected");
	}

	if(buffering_mode_) {
		command_buffer_ += command;
		command_buffer_ += '\n';
		return "";
	}
	socket_.send_action(command);

	return command;
}

void
tapplication::connect(const std::string& server)
{
	/** @todo The server string should be a (hostname|ip address) [port] */

	VALIDATE(!connected_);

	socket_.connect(server, "2048");
}

void
tapplication::asio_callback_signal()
{
	LOG_I("Application: received a signal.\n");

	logging::module::set_sync_mode();
	io_service_.stop();

	for(auto& thread : threads_) {
		thread.join();
	}

	threads_.clear();
}

void
tapplication::asio_callback_socket_connect(
		const boost::system::error_code& error)
{
	LOG_T(__PRETTY_FUNCTION__, ": error »", error.message(), "«.\n");

	if(!error) {
		connected_ = true;
	}

	socket_.receive();
}

void
tapplication::asio_callback_socket_send(
		  const boost::system::error_code& error
		, const communication::tmessage &message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« message »", message.contents()
			, "«.\n");
}

void
tapplication::asio_callback_socket_receive(
		  const boost::system::error_code& error
		, const communication::tmessage *message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« message »", message ? message->contents() : "NULL"
			, "«.\n");

	if(error) {
		return;
	}

	output_.send_action(message->contents());
}
