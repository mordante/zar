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

#define ENUM_ENABLE_STREAM_OPERATORS_IMPLEMENTATION
#define ENUM_TYPE ::communication::tconnection::tprotocol
#define ENUM_LIST                                                             \
ENUM(direct,                      "direct");                                  \
ENUM(line,                        "line");                                    \
ENUM(telnet,                      "telnet");                                  \
ENUM(basic,                       "basic");                                   \
ENUM(compressed,                  "compressed");                              \

#include "modules/communication/connection.hpp"

#include "lib/exception/validate.tpp"
#include "modules/logging/log.hpp"

ENUM_DEFINE_STREAM_OPERATORS(ENUM_TYPE)

namespace communication {

tconnection::tmessage::tmessage()
	: tmessage(static_cast<ttype>(-1), 0, "")
{
}

tconnection::tmessage::tmessage(
		  const ttype type__
		, const uint32_t id__
		, const std::string& message__)
	: type(type__)
	, id(id__)
	, message(message__)
{
}

tconnection::~tconnection()
{
	set_sync_mode();
}

void
tconnection::set_async_mode(boost::asio::io_service& io_service)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	set_sync_mode();
	strand_ = new boost::asio::io_service::strand(io_service);
	own_strand_ = true;
}

void
tconnection::set_async_mode(boost::asio::io_service::strand& strand__)
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	set_sync_mode();
	strand_ = &strand__;
}

void
tconnection::set_sync_mode()
{
	LOG_T(__PRETTY_FUNCTION__, ".\n");

	if(own_strand_) {
		delete strand_;
		own_strand_ = false;
	}

	own_strand_ = nullptr;
}

uint32_t
tconnection::network_buffer_to_host(const char* value)
{
	return ntohl(*reinterpret_cast<const uint32_t*>(value));
}

tconnection::tmessage
tconnection::decode(std::string data)
{
	LOG_T(__PRETTY_FUNCTION__, ": data »", data, "«.\n");

	tmessage message;

	switch(protocol_) {
		case tprotocol::direct :
			message.id = 0;
			break;

		case tprotocol::line :
			message.id = 0;
			data.resize(data.size() - 1);
			break;

		case tprotocol::telnet :
			message.id = 0;
			data.resize(data.size() - 2);
			break;

		case tprotocol::basic :
			if(data.size() < 5) {
				throw;
			}
			switch(data[0]) {
				case 'A' :
					message.type = tmessage::ttype::action;
					break;

				case 'R' :
					message.type = tmessage::ttype::reply;
					break;

				default:
					throw;
			}
			message.id = network_buffer_to_host(&data[1]);
			data.erase(0, 5);
			break;

		case tprotocol::compressed :
			NOT_IMPLEMENTED_YET;
			break;
	}

	message.message = data;
	return message;
}

std::string
tconnection::host_to_network_string(const uint32_t value)
{
	return std::string(reinterpret_cast<char*>(htonl(value)), 4);
}

tconnection::tmessage
tconnection::encode(const tmessage::ttype type
		, uint32_t id
		, std::string data)
{
	LOG_T(__PRETTY_FUNCTION__, ": data »", data, "«.\n");

	switch(protocol_) {
		case tprotocol::direct :
			id = 0;
			break;

		case tprotocol::line :
			id = 0;
			data += '\n';
			break;

		case tprotocol::telnet :
			id = 0;
			data += "\r\n";
			break;

		case tprotocol::basic :
			switch(type) {
				case tmessage::ttype::action :
					data = "A" + host_to_network_string(id) + data;
					break;

				case tmessage::ttype::reply :
					data = "R" + host_to_network_string(id) + data;
					break;
			}
//			if(data.size() > max - 4) ..
			data = host_to_network_string(static_cast<uint32_t>(data.size()))
					+ data;
			break;

		case tprotocol::compressed :
			NOT_IMPLEMENTED_YET;
			break;
	}

	return tmessage(type, id, data);
}

boost::asio::io_service::strand*
tconnection::strand()
{
	return strand_;
}

void
tconnection::set_protocol(const tprotocol protocol__)
{
	LOG_T(__PRETTY_FUNCTION__, ": protocol »", protocol__, "«.\n");

	protocol_ = protocol__;
}

tconnection::tprotocol
tconnection::get_protocol() const
{
	return protocol_;
}


} // namespace communication
