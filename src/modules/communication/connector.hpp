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

#ifndef MODULES_COMMUNICATION_CONNECTOR_HPP_INCLUDED
#define MODULES_COMMUNICATION_CONNECTOR_HPP_INCLUDED

#include "modules/communication/connection.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>

#include <functional>

namespace communication {

class tconnector_
	: public virtual tconnection
{
public:

	/***** ***** Constructor, destructor, assignment. ***** *****/

	tconnector_() = default;

	virtual ~tconnector_();

	tconnector_&
	operator=(const tconnector_&) = delete;
	tconnector_(const tconnector_&) = delete;

	tconnector_&
	operator=(tconnector_&&) = default;
	tconnector_(tconnector_&&) = default;


	/***** ***** Operators. ***** *****/

	void
	connect(const std::string& hostname, const std::string& service);


	/***** ***** Setters, getters. ***** *****/

	void
	set_connect_handler(const tconnect_handler& handler__);

protected:

	/***** ***** Members. ***** *****/

	tconnect_handler connect_handler_{};

	virtual void
	async_query(const std::string hostname, const std::string service) = 0;
};

template<class InternetProtocol>
class tconnector
	: virtual public InternetProtocol::socket
	, public tconnector_
{
public:

	explicit tconnector(boost::asio::io_service& io_service)
		: InternetProtocol::socket(io_service)
		, tconnector_()
		, resolver_(io_service)
	{
	}

	virtual ~tconnector() {} /* default gives linker errors. */

	tconnector&
	operator=(const tconnector&) = delete;
	tconnector(const tconnector&) = delete;

	tconnector&
	operator=(tconnector&&) = default;
	tconnector(tconnector&&) = default;

	typename InternetProtocol::resolver resolver_;
	typename InternetProtocol::resolver::query query_{"", ""};

	virtual void
	async_query(const std::string hostname, const std::string service) override
	{
		query_ = typename InternetProtocol::resolver::query(
				  InternetProtocol::v4()
				, hostname
				, service);
		typedef std::function<void(
					  const boost::system::error_code&
					, typename InternetProtocol::resolver::iterator
				)>
				thandler;

		strand_execute([&](thandler&& handler)
				  {
					  resolver_.async_resolve(query_, handler);
				  }
				, std::bind(
					  &tconnector::asio_callback_resolve
					, this
					, std::placeholders::_1
					, std::placeholders::_2));
	}

	void
	asio_callback_resolve(
			  const boost::system::error_code& error
			, typename InternetProtocol::resolver::iterator iterator)
	{
		if(error) {
			if(connect_handler_) {
				connect_handler_(error);
				return;
			}
		}

		async_connect(iterator);

	}

	void
	async_connect(typename InternetProtocol::resolver::iterator iterator)
	{
		typedef std::function<void(
					  const boost::system::error_code&
					, typename InternetProtocol::resolver::iterator
				)>
				thandler;

		strand_execute([&](thandler&& handler)
				  {
					  boost::asio::async_connect(
							  dynamic_cast<typename InternetProtocol::socket&>(
								  *this)
							, iterator
							, handler);
				  }
				, std::bind(
					  &tconnector::asio_callback_connect
					, this
					, std::placeholders::_1
					, iterator));
	}

	void
	asio_callback_connect(
			  const boost::system::error_code& error
			, typename InternetProtocol::resolver::iterator iterator)
	{
		if(!error) {
			if(connect_handler_) {
				connect_handler_(error);
			}
			return;
		}

		++iterator;
		if(iterator == typename InternetProtocol::resolver::iterator()) {
			if(connect_handler_) {
				connect_handler_(error);
			}
			return;
		}

		async_connect(iterator);
	}
};

extern template class tconnector<boost::asio::ip::tcp>;
typedef tconnector<boost::asio::ip::tcp> tconnector_socket;

} // namespace communication

#endif

