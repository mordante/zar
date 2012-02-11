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

#ifndef MODULES_LOBBY_SESSION_HPP_INCLUDED
#define MODULES_LOBBY_SESSION_HPP_INCLUDED

#include "modules/communication/tcp_socket.hpp"

namespace lobby {

class tsession final
{
public:

	/***** ***** Types. ***** *****/

	enum class tstatus
	{
		  wait_for_connection
		, connected
		, disconnected
		, reapable
	};

	enum class tmode
	{
		  connected /* Directly after connecting. */
		, lobby /* After logging in */
		, creating_game /* creator */
//		, joining_game /* others */
//		, playing_game
	};


	/***** ***** Constructor, destructor, assignment. ***** *****/

	explicit tsession(boost::asio::io_service& io_service);

	~tsession() = default;

	tsession&
	operator=(const tsession&) = delete;
	tsession(const tsession&) = delete;

	tsession&
	operator=(tsession&&) = delete;
	tsession(tsession&&) = delete;


	/***** ***** Operators. ***** *****/

	void
	accept(boost::asio::ip::tcp::acceptor& acceptor);

	void
	send(const std::string& data);

	void
	receive();

	/***** ***** Setters, getters. ***** *****/

	tstatus
	get_status() const;

	void
	set_status(const tstatus status__);

	void
	set_mode(const tmode mode__);

	tmode
	get_mode() const;

	std::string
	get_id() const;

	void
	set_id(const std::string& id__);

	void
	set_accept_handler(communication::taccept_handler accept_handler__);

	void
	set_send_handler(communication::tsend_handler send_handler__);

	void
	set_receive_handler(communication::treceive_handler receiv_handler__);

private:

	/***** ***** Members. ***** *****/

	/***** Status. *****/

	tstatus status_{tstatus::wait_for_connection};

	unsigned protocol_version{1};

	tmode mode_{tmode::connected};

	std::string id_{};


	/***** Data transmission. *****/

	communication::ttcp_socket socket_;

	/** The accept handler for the user of this class .*/
	communication::taccept_handler accept_handler_{};

	/** The send handler for the user of this class .*/
	communication::tsend_handler send_handler_{};

	/** The receive handler for the user of this class .*/
	communication::treceive_handler receive_handler_{};

	/** The accept handler for the session .*/
	void
	session_accept_handler(const boost::system::error_code& error);

	/** The receive handler for the session .*/
	void
	session_receive_handler(
			  const boost::system::error_code& error
			, const size_t bytes_transferred
			, const communication::tmessage* message);

	/** The send handler for the session .*/
	void
	session_send_handler(
			  const boost::system::error_code& error
			, const size_t bytes_transferred
			, const communication::tmessage& message);
};

} // namespace lobby

#endif
