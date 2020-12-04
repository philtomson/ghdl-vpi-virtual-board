/* Copyright (C) 2020 Théotime Bollengier <theotime.bollengier@ensta-bretagne.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "VBMessage.hh"


VBMessage::VBMessage() :
	m_type(VBMessage::MSG_NONE),
	m_value(0),
	m_io_name(VBMessage::NONE),
	m_pointer(nullptr)
{
}


VBMessage::VBMessage(const VBMessage& other) :
	m_type(other.m_type),
	m_value(other.m_value),
	m_io_name(other.m_io_name),
	m_pointer(other.m_pointer)
{
}


VBMessage::VBMessage(VBMessage::message_type_t type) :
	m_type(type),
	m_value(0),
	m_io_name(VBMessage::NONE),
	m_pointer(nullptr)
{
}


VBMessage::VBMessage(VBMessage::message_type_t type, unsigned int value) :
	m_type(type),
	m_value(value),
	m_io_name(VBMessage::NONE),
	m_pointer(nullptr)
{
}


VBMessage::VBMessage(message_type_t type, void *ptr) :
	m_type(type),
	m_value(0),
	m_io_name(VBMessage::NONE),
	m_pointer(ptr)
{
}


VBMessage::VBMessage(VBMessage::message_type_t type, unsigned int value, VBMessage::IO_name_t io_name) :
	m_type(type),
	m_value(value),
	m_io_name(io_name),
	m_pointer(nullptr)
{
}


VBMessage::message_type_t VBMessage::type() const
{
	return m_type;
}


unsigned int VBMessage::value() const
{
	return m_value;
}


VBMessage::IO_name_t VBMessage::io_name() const
{
	return m_io_name;
}


ModuleInstance* VBMessage::module_instance() const
{
	return (ModuleInstance*)m_pointer;
}


ModuleNet* VBMessage::module_net() const
{
	return (ModuleNet*)m_pointer;
}


VBMessage VBMessage::exit()
{
	VBMessage msg(VBMessage::MSG_EXIT);
	return msg;
}


VBMessage VBMessage::run()
{
	VBMessage msg(VBMessage::MSG_RUN);
	return msg;
}


VBMessage VBMessage::run_n(unsigned int n)
{
	VBMessage msg(VBMessage::MSG_RUN_N, n);
	return msg;
}


VBMessage VBMessage::stop()
{
	VBMessage msg(VBMessage::MSG_STOP);
	return msg;
}


VBMessage VBMessage::set_freq(unsigned int n)
{
	VBMessage msg(VBMessage::MSG_SET_FREQ, n);
	return msg;
}


VBMessage VBMessage::io_changed(VBMessage::IO_name_t io_name, unsigned int value)
{
	VBMessage msg(VBMessage::MSG_IO_CHANGED, value, io_name);
	return msg;
}


VBMessage VBMessage::clock()
{
	VBMessage msg(VBMessage::MSG_CLOCK);
	return msg;
}


VBMessage VBMessage::stopped()
{
	VBMessage msg(VBMessage::MSG_STOPPED);
	return msg;
}


VBMessage VBMessage::running()
{
	VBMessage msg(VBMessage::MSG_RUNNING);
	return msg;
}


VBMessage VBMessage::gui_started()
{
	VBMessage msg(VBMessage::MSG_GUI_STARTED);
	return msg;
}


VBMessage VBMessage::read_module_nets(ModuleInstance *mod)
{
	VBMessage msg(VBMessage::MSG_READ_MODULE_NETS, (void*)mod);
	return msg;
}


VBMessage VBMessage::module_nets_read(ModuleInstance *mod)
{
	VBMessage msg(VBMessage::MSG_MODULE_NETS_READ, (void*)mod);
	return msg;
}


VBMessage VBMessage::read_net(ModuleNet *net)
{
	VBMessage msg(VBMessage::MSG_READ_NET, (void*)net);
	return msg;
}


VBMessage VBMessage::net_read(ModuleNet *net)
{
	VBMessage msg(VBMessage::MSG_NET_READ, (void*)net);
	return msg;
}


const char *VBMessage::type_to_s() const
{
	switch (m_type) {
		case VBMessage::MSG_NONE:
			return "MSG_NONE";
		case VBMessage::MSG_EXIT:
			return "MSG_EXIT";
		case VBMessage::MSG_RUN:
			return "MSG_RUN";
		case VBMessage::MSG_RUN_N:
			return "MSG_RUN_N";
		case VBMessage::MSG_STOP:
			return "MSG_STOP";
		case VBMessage::MSG_SET_FREQ:
			return "MSG_SET_FREQ";
		case VBMessage::MSG_IO_CHANGED:
			return "MSG_IO_CHANGED";
		case VBMessage::MSG_CLOCK:
			return "MSG_CLOCK";
		case VBMessage::MSG_STOPPED:
			return "MSG_STOPPED";
		case VBMessage::MSG_RUNNING:
			return "MSG_RUNNING";
		case VBMessage::MSG_GUI_STARTED:
			return "MSG_GUI_STARTED";
		case VBMessage::MSG_READ_MODULE_NETS:
			return "MSG_READ_MODULE_NETS";
		case VBMessage::MSG_MODULE_NETS_READ:
			return "MSG_MODULE_NETS_READ";
		case VBMessage::MSG_READ_NET:
			return "MSG_READ_NET";
		case VBMessage::MSG_NET_READ:
			return "MSG_NET_READ";
	}

	return "Unknown MSG";
}


