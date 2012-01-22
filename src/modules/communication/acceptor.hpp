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

#ifndef MODULES_COMMUNICATION_ACCEPTOR_HPP_INCLUDED
#define MODULES_COMMUNICATION_ACCEPTOR_HPP_INCLUDED

#include "modules/communication/connection.hpp"

#include <boost/asio/ip/tcp.hpp>

namespace communication {

class tacceptor_
	: public virtual tconnection
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tacceptor_() = default;

	virtual ~tacceptor_();

	tacceptor_&
	operator=(const tacceptor_&) = delete;
	tacceptor_(const tacceptor_&) = delete;

	tacceptor_&
	operator=(tacceptor_&&) = default;
	tacceptor_(tacceptor_&&) = default;


	/***** ***** Operators. ***** *****/

	/***** ***** Setters, getters. ***** *****/

	void
	set_accept_handler(const taccept_handler& handler__);

protected:

	/***** ***** Members. ***** *****/

	taccept_handler accept_handler_{};

	void
	asio_accept_callback(const boost::system::error_code& error);
};

template<class InternetProtocol>
class tacceptor
	: virtual public InternetProtocol::socket
	, public tacceptor_
{
public:

	explicit tacceptor(boost::asio::io_service& io_service)
		: InternetProtocol::socket(io_service)
	{
	}

	virtual ~tacceptor() {} /* default gives linker errors. */

	tacceptor&
	operator=(const tacceptor&) = delete;
	tacceptor(const tacceptor&) = delete;

	tacceptor&
	operator=(tacceptor&&) = default;
	tacceptor(tacceptor&&) = default;

	void
	accept(typename InternetProtocol::acceptor& acceptor);
private:
};

extern template class tacceptor<boost::asio::ip::tcp>;
typedef tacceptor<boost::asio::ip::tcp> tacceptor_socket;

} // namespace communication

#endif

