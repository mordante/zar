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

#define LOGGER_DEFINE_MODULE_LOGGER_MACROS "communication"

#include "modules/communication/message.hpp"

#include "lib/exception/validate.tpp"
#include "modules/logging/log.hpp"

#include <arpa/inet.h>

namespace communication {

tmessage::tmessage(const tprotocol protocol, const std::string& encoded_message)
	: type_(ttype::reply) /* Notification. */
	, id_(0)
	, contents_()
{
	LOG_T(__PRETTY_FUNCTION__, ": encoded_message »", encoded_message, "«.\n");

	switch(protocol) {
		case tprotocol::direct :
			contents_ = encoded_message;
			break;

		case tprotocol::line :
			contents_ = encoded_message.substr(0, encoded_message.size() - 1);
			break;

		case tprotocol::telnet :
			contents_ = encoded_message.substr(0, encoded_message.size() - 2);
			break;

		case tprotocol::basic :
			if(encoded_message.size() < 5) {
				throw lib::texception(
						  lib::texception::ttype::protocol_error
						, lib::concatenate(
							    "A basic message is too small for its header."
							  , " The size is »"
							  , encoded_message.size()
							  , "«"));
			}
			switch(encoded_message[0]) {
				case 'A' :
					type_ = ttype::action;
					break;

				case 'R' :
					type_ = ttype::reply;
					break;

				default:
					throw lib::texception(
							  lib::texception::ttype::invalid_value
							, lib::concatenate(
								    "Found unexpected message type »"
								  , encoded_message[0]
								  , "«"));
			}
			id_ = network_buffer_to_host(&encoded_message[1]);
			contents_ = encoded_message.substr(5);
			break;

		case tprotocol::compressed :
			NOT_IMPLEMENTED_YET;
			break;
	}
}

tmessage::tmessage(
		  const ttype type__
		, const uint32_t id__
		, const std::string& contents__)
	: type_(type__)
	, id_(id__)
	, contents_(contents__)
{
}

std::string
tmessage::encode(const tprotocol protocol) const
{
	LOG_T(__PRETTY_FUNCTION__, ": protocol »", protocol, "«.\n");

	std::string result;

	switch(protocol) {
		case tprotocol::direct :
			break;

		case tprotocol::line :
			result = contents_ + '\n';
			break;

		case tprotocol::telnet :
			result = contents_ + "\r\n";
			break;

		case tprotocol::basic :
			switch(type_) {
				case tmessage::ttype::action :
					result = "A" + host_to_network_string(id_) + contents_;
					break;

				case tmessage::ttype::reply :
					result = "R" + host_to_network_string(id_) + contents_;
					break;
			}
//			if(data.size() > max - 4) ..
			result = host_to_network_string(
						static_cast<uint32_t>(result.size()))
					+ result;
			break;

		case tprotocol::compressed :
			NOT_IMPLEMENTED_YET;
			break;
	}

	return result;
}

tmessage::ttype
tmessage::type() const
{
	return type_;
}

uint32_t
tmessage::id() const
{
	return id_;
}

const std::string&
tmessage::contents() const
{
	return contents_;
}

uint32_t
network_buffer_to_host(const char value[4])
{
	return ntohl(*reinterpret_cast<const uint32_t*>(value));
}

std::string
host_to_network_string(uint32_t value)
{
	value = htonl(value);
	return std::string(reinterpret_cast<char*>(&value), 4);
}

} // namespace communication
