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
 * Contains the messages used in the communication protocol.
 *
 * Depending on the actual @ref communication::tprotocol a message needs to
 * be encoded and decoded differently. This file contains a class and some
 * helper functions to aid this process.
 */

#ifndef MODULES_COMMUNICATION_MESSAGE_HPP_INCLUDED
#define MODULES_COMMUNICATION_MESSAGE_HPP_INCLUDED

#include "modules/communication/types.hpp"

#include <string>

namespace communication {

class tmessage final
{
public:

	/***** ***** Types. ***** *****/

	/**
	 * The type of the message.
	 */
	enum class ttype
	{
		  action
		, reply
	};


	/***** ***** Constructor, destructor, assignment. ***** *****/

	/**
	 * Constructor.
	 *
	 * This constructor takes a raw message transferred over a communication
	 * link, e.g. a network socket and constructs a message from it using in
	 * the requested @p protocol.
	 *
	 * @param protocol               The procotol to use to decode the
	 *                               message.
	 * @param encoded_message        The raw message to decode.
	 */
	tmessage(const tprotocol protocol, const std::string& encoded_message);

	tmessage(const ttype type__
			, const uint32_t id__
			, const std::string& contents__);

	~tmessage() = default;

	tmessage&
	operator=(const tmessage&) = default;
	tmessage(const tmessage&) = default;

	tmessage&
	operator=(tmessage&&) = default;
	tmessage(tmessage&&) = default;


	/***** ***** Operators. ***** *****/

	/**
	 * Encodes the contents of a message.
	 *
	 * After encoding the message can be transferred over a communication
	 * link e.g. a network socket.
	 *
	 * @param protocol               The protocol, which shall be used to
	 *                               encode the message.
	 *
	 * @returns                      The encoded message.
	 */
	std::string
	encode(const tprotocol protocol) const;


	/***** ***** Setters, getters. ***** *****/

	ttype
	type() const;

	uint32_t
	id() const;

	const std::string&
	contents() const;


private:

	/***** ***** Members. ***** *****/

	/** The type of the message. */
	ttype type_;

	/** The id of the message. */
	uint32_t id_;

	/** The actual message. */
	std::string contents_;
};

/**
 * Converts a 4-byte buffer in network order to an @c uint32_t in host order.
 *
 * @param value                   The buffer to convert.
 *
 * @returns                       The converted value.
 */
uint32_t
network_buffer_to_host(const char value[4]);

/**
 * Converts an uint32_t in host order to a 4-byte buffer in network order.
 *
 * @param value                   The @c uint32_t to convert.
 *
 * @returns                       A string with the converted value.
 */
std::string
host_to_network_string(const uint32_t value);

} // namespace communication

#endif
