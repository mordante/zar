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

#ifndef MODULES_COMMUNICATION_FILE_HPP_INCLUDED
#define MODULES_COMMUNICATION_FILE_HPP_INCLUDED

#include "modules/communication/detail/receiver.hpp"
#include "modules/communication/detail/sender.hpp"

namespace communication {

class tfile final
/*	: public treceiver_file
	, public tsender_file
*/{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tfile(boost::asio::io_service& io_service, const int fd);

	virtual ~tfile();

	tfile&
	operator=(const tfile&) = delete;
	tfile(const tfile&) = delete;

	tfile&
	operator=(tfile&&) = default;
	tfile(tfile&&) = default;

	/***** ***** Operators. ***** *****/

	void
	strand_enable(boost::asio::io_service& io_service);

	void
	strand_enable(boost::asio::io_service::strand& strand__);

	void
	strand_disable();

	void
	receive();

	uint32_t
	send_action(const std::string& message);

	void
	send_reply(const uint32_t id__, const std::string& message);

	/***** ***** Setters, getters. ***** *****/

	void
	set_protocol(const tprotocol protocol__);

	tprotocol
	get_protocol() const;

	void
	set_receive_handler(const treceive_handler& handler__);

	void
	set_send_handler(const tsend_handler& handler__);

private:

	/***** ***** Operators. ***** *****/

	detail::tconnection connection_{};

	boost::asio::posix::stream_descriptor file_;

	detail::treceiver_file receiver_;

	detail::tsender_file sender_;
};

} // namespace communication

#endif

