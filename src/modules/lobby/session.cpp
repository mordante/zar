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

#include "modules/lobby/session.hpp"

#include "modules/logging/log.hpp"

namespace lobby {

tsession::tsession(boost::asio::io_service& io_service)
	: socket_(io_service)
{
	socket_.set_accept_handler(std::bind(
			  &tsession::session_accept_handler
			, this
			, std::placeholders::_1));

	socket_.set_receive_handler(std::bind(
			  &tsession::session_receive_handler
			, this
			, std::placeholders::_1
			, std::placeholders::_2
			, std::placeholders::_3));

	socket_.set_send_handler(std::bind(
			  &tsession::session_send_handler
			, this
			, std::placeholders::_1
			, std::placeholders::_2
			, std::placeholders::_3));

	socket_.strand_enable(io_service);
}

void
tsession::accept(boost::asio::ip::tcp::acceptor& acceptor)
{
	socket_.accept(acceptor);
}

void
tsession::send(const std::string& data)
{
	socket_.send_action(data);
}

void
tsession::receive()
{
	socket_.receive();
}

tsession::tstatus
tsession::get_status() const
{
	return status_;
}

void
tsession::set_status(const tstatus status__)
{
	// setter needed??
	status_ = status__;
}

void
tsession::set_mode(const tmode mode__)
{
	mode_ = mode__;
}

tsession::tmode
tsession::get_mode() const
{
	return mode_;
}

std::string
tsession::get_id() const
{
	return id_;
}

void
tsession::set_id(const std::string& id__)
{
	id_ = id__;
}

void
tsession::set_accept_handler(communication::taccept_handler accept_handler__)
{
	accept_handler_ = accept_handler__;
}

void
tsession::set_send_handler(communication::tsend_handler send_handler__)
{
	send_handler_ = send_handler__;
}

void
tsession::set_receive_handler(communication::treceive_handler receiv_handler__)
{
	receive_handler_ = receiv_handler__;
}

void
tsession::session_accept_handler(const boost::system::error_code& error)
{
	LOG_T(__PRETTY_FUNCTION__, ": error »", error.message(), "«.\n");

	if(error) {
		set_status(tstatus::reapable);
		return;

	}

	if(accept_handler_) {
		accept_handler_(error);
	}
}

void
tsession::session_receive_handler(
		  const boost::system::error_code& error
		, const size_t bytes_transferred
		, const communication::tmessage* message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« bytes_transferred »", bytes_transferred
			, "« message.id »", message ? message->id() : 0
			, "« message.data »", message ? message->contents() : "NULL"
			, "«.\n");

	if(receive_handler_) {
		receive_handler_(error, bytes_transferred, message);
	}

	if(error) {
		if(error == boost::asio::error::eof) {
			LOG_I("Client disconnected.\n");
			status_ = tstatus::disconnected;
		} else if(error) {
			LOG_E("Error »"
					, error.message()
					, "« while receiving data, connection closed.\n");
			status_ = tstatus::disconnected;
		}
		return;
	}
}

void
tsession::session_send_handler(
		  const boost::system::error_code& error
		, const size_t bytes_transferred
		, const communication::tmessage& message)
{
	LOG_T(__PRETTY_FUNCTION__
			, ": error »", error.message()
			, "« bytes_transferred »", bytes_transferred
			, "« message.id »", message.id()
			, "« message.data »", message.contents()
			, "«.\n");

	if(error) {
//		LOG_E(); eof or is it pipe???
		socket_.close();
		status_ = tstatus::reapable;
		return;
	}

}

} // namespace lobby
