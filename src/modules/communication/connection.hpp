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

/**
 * @file
 * Base class for the communication module.
 *
 * The module is still under heavy development and the class hierarchy has
 * not yet been cast in stone.
 *
 * @todo Finish and document the communication module.
 */

#ifndef MODULES_COMMUNICATION_CONNECTION_HPP_INCLUDED
#define MODULES_COMMUNICATION_CONNECTION_HPP_INCLUDED

#include "lib/strand/strand.hpp"

namespace communication {

class tconnection
	: public lib::tstrand
{
public:

	/***** ***** Types. ***** *****/

	enum class tprotocol
	{
		/** No formatting. */
		  direct

		/** Line based on Linux. */
		, line

		/** Line based on telnet. */
		, telnet

		/** Data has its length prefixed. */
		, basic

		/** Data has its length prefixed and compressed. */
		, compressed
	};

	struct tmessage final
	{
		/***** ***** Types. ***** *****/

		enum class ttype
		{
			  action
			, reply
		};


		/***** ***** Constructor, destructor, assignment. ***** *****/

		/**
		 * Constructor.
		 *
		 * Creates an invalid object, the type is nonsense.
		 */
		tmessage();

		tmessage(const ttype type__
				, const uint32_t id__
				, const std::string& message__);

		~tmessage() = default;

		tmessage&
		operator=(const tmessage&) = default;
		tmessage(const tmessage&) = default;

		tmessage&
		operator=(tmessage&&) = default;
		tmessage(tmessage&&) = default;


		/***** ***** Members. ***** *****/

		ttype type;
		uint32_t id;
		/** rename field to data? */
		std::string message;
	};

	typedef std::function<void(
				  const boost::system::error_code& error
			)>
			taccept_handler;

	typedef std::function<void(
				  const boost::system::error_code& error
			)>
			tconnect_handler;

	typedef std::function<void(
				  const boost::system::error_code& error
				, const size_t bytes_transferred
				, const tmessage& message
			)>
			treceive_handler;

	typedef std::function<void(
				  const boost::system::error_code& error
				, const size_t bytes_transferred
				, const tmessage& message
			)>
			tsend_handler;

	typedef
			std::function<void(const boost::system::error_code&)>
			terror_handler;

	typedef std::function<void(
				  const boost::system::error_code& error
				, const size_t bytes_transferred
			)>
			thandler;


	/***** ***** Constructor, destructor, assignment. ***** *****/

	tconnection() = default;

	virtual ~tconnection();

	tconnection&
	operator=(const tconnection&) = delete;
	tconnection(const tconnection&) = delete;

	tconnection&
	operator=(tconnection&&) = default;
	tconnection(tconnection&&) = default;


	/***** ***** Operators. ***** *****/

	static uint32_t
	network_buffer_to_host(const char* value);

	tmessage
	decode(std::string data);

	static std::string
	host_to_network_string(const uint32_t value);

	tmessage
	encode(const tmessage::ttype type, uint32_t id, std::string data);


	/***** ***** Setters, getters. ***** *****/

	void
	set_protocol(const tprotocol protocol__);

	tprotocol
	get_protocol() const;

	void
	set_error_handler(const terror_handler& error_handler__);

private:

	/***** ***** Members. ***** *****/

	tprotocol protocol_{tprotocol::telnet};

	terror_handler error_handler_{};
};

} // namespace communication

#include "lib/string/enumerate.tpp"

ENUM_DECLARE_STREAM_OPERATORS(::communication::tconnection::tprotocol)

#endif

