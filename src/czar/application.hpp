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

#ifndef CLIENT_APPLICATION_HPP_INCLUDED
#define CLIENT_APPLICATION_HPP_INCLUDED

#include "modules/communication/file.hpp"
#include "modules/communication/tcp_socket.hpp"

#include <boost/asio/signal_set.hpp>

#include <thread>

struct tapplication final
{
public:
	/***** ***** Types. ***** *****/

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tapplication();

	~tapplication() = default;

	tapplication&
	operator=(const tapplication&) = delete;
	tapplication(const tapplication&) = delete;

	tapplication&
	operator=(tapplication&&) = delete;
	tapplication(tapplication&&) = delete;

	/***** ***** Operators. ***** *****/

	/***** ***** Setters, getters. ***** *****/


private:

	/***** ***** Members. ***** *****/

	/***** Status. *****/

	/**
	 * Is the buffering mode enabled?
	 *
	 * Normally an end of line is the trigger to send a command. Some commands
	 * are allowed to send multiple lines, so then the lines are buffered until
	 * the proper time so send the entire buffer.
	 */
	bool buffering_mode_{false};

	std::string command_buffer_{};

	/***** Data transmission. *****/

	bool connected_{false};

	boost::asio::io_service io_service_{};

	boost::asio::signal_set signals_{io_service_, SIGINT, SIGTERM};

	communication::tfile input_{io_service_, STDIN_FILENO};

	communication::tfile output_{io_service_, STDOUT_FILENO};

	communication::ttcp_socket socket_{io_service_};

	/**
	 * The thread pool.
	 *
	 * The application uses multiple threads, not since it's really required
	 * but to get a feeling how well multi-threaded code works with asio.
	 */
	std::vector<std::thread> threads_{};

	void
	run();

	void
	loop();

	void
	execute(const boost::system::error_code& error
			, const communication::tmessage* message);

	std::string
	execute_client(const std::string& command);

	std::string
	execute_server(const std::string& command);

	void
	connect(const std::string& server);

	/***** Asio callbacks. *****/

	void
	asio_callback_signal();

	void
	asio_callback_socket_connect(const boost::system::error_code& error);

	void
	asio_callback_socket_send(
			  const boost::system::error_code& error
			, const communication::tmessage &message);

	void
	asio_callback_socket_receive(
			  const boost::system::error_code& error
			, const communication::tmessage *message);
};
#endif

