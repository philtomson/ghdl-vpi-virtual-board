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

#ifndef VB_MESSAGE_HH
#define VB_MESSAGE_HH

#include "Instance.hh"


class VBMessage {
public:
	typedef enum {
		NONE = 0,
		SWITCHES,
		PUSH_BUTTON_RSTN,
		PUSH_BUTTON_CENTER,
		PUSH_BUTTON_UP,
		PUSH_BUTTON_DOWN,
		PUSH_BUTTON_RIGHT,
		PUSH_BUTTON_LEFT,
		LEDS,
		RGB_LED_0,
		RGB_LED_1,
		DISPLAY_0,
		DISPLAY_1,
		DISPLAY_2,
		DISPLAY_3,
		DISPLAY_4,
		DISPLAY_5,
		DISPLAY_6,
		DISPLAY_7
	} IO_name_t;

	typedef enum {
		MSG_NONE = 0,
		MSG_EXIT,
		MSG_RUN,
		MSG_RUN_N, // integer
		MSG_STOP,
		MSG_SET_FREQ, // integer
		MSG_IO_CHANGED, // IO_name_t, integer
		MSG_CLOCK,
		MSG_STOPPED,
		MSG_RUNNING,
		MSG_GUI_STARTED,
		MSG_READ_MODULE_NETS, // pointer points to a ModuleInstance
		MSG_MODULE_NETS_READ, // pointer points to a ModuleInstance
//		MSG_WRITE_NET, // pointer points to a ModuleNet
	} message_type_t;

private:
	message_type_t m_type;
	unsigned int   m_value;
	IO_name_t      m_io_name;
	void          *m_pointer;

public:
	VBMessage();
	VBMessage(const VBMessage& other);
	VBMessage(message_type_t type);
	VBMessage(message_type_t type, unsigned int value);
	VBMessage(message_type_t type, void *ptr);
	VBMessage(message_type_t type, unsigned int value, IO_name_t io_name);

	message_type_t type() const;
	unsigned int   value() const;
	IO_name_t      io_name() const;
	const char*    type_to_s() const;
	ModuleInstance* module_instance() const;
//	ModuleNet* module_net() const;

	static VBMessage exit();
	static VBMessage run();
	static VBMessage run_n(unsigned int n);
	static VBMessage stop();
	static VBMessage set_freq(unsigned int n);
	static VBMessage io_changed(IO_name_t io_name, unsigned int value);
	static VBMessage clock();
	static VBMessage stopped();
	static VBMessage running();
	static VBMessage gui_started();
	static VBMessage read_module_nets(ModuleInstance *mod);
	static VBMessage module_nets_read(ModuleInstance *mod);
//	static VBMessage write_net(ModuleNet *net);
};

#endif /* VB_MESSAGE_HH */
