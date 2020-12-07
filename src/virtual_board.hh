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

#ifndef VIRTUAL_BOARD_HH
#define VIRTUAL_BOARD_HH

#include <vpi_user.h>
#include <gtkmm/application.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <cstdint>
#include <time.h>
#include "board_window.hh"
#include "VBMessage.hh"
#include "Instance.hh"


class VirtualBoard {
private:
	std::thread *m_thread;
	VBWindow    *m_window;
	double       m_time_resolution;
	std::mutex   m_to_vpi_mutex;
	std::condition_variable m_to_vpi_condvar;
	std::queue<VBMessage> m_to_vpi_queue;
	std::mutex   m_to_gui_mutex;
	std::queue<VBMessage> m_to_gui_queue;
	double       m_period;
	timer_t      m_timer;
	bool         m_timer_is_running;

public:
	long         cycles_to_go;
	std::string  design_top_unit_name;
	std::string  simulator_name;
	std::string  simulator_version;
	ModuleInstance top_module;
	vpiHandle    clk_net;
	vpiHandle    rstn_net;
	vpiHandle    switches_net;
	vpiHandle    button_c_net;
	vpiHandle    button_u_net;
	vpiHandle    button_d_net;
	vpiHandle    button_r_net;
	vpiHandle    button_l_net;
	vpiHandle    leds_net;
	vpiHandle    display0_net;
	vpiHandle    display1_net;
	vpiHandle    display2_net;
	vpiHandle    display3_net;
	vpiHandle    display4_net;
	vpiHandle    display5_net;
	vpiHandle    display6_net;
	vpiHandle    display7_net;
	vpiHandle    anodes_net;
	vpiHandle    cathodes_net;
	vpiHandle    rgb0_net;
	vpiHandle    rgb1_net;
	int          display_interface; // 0: none, 1: direct, 2: anodes multiplexing

public:
	VirtualBoard();
	~VirtualBoard();

	void start_gui_thread();

	void set_time_resolution(int res);
	s_vpi_time get_time(double t);
	double half_period();

	void set_timer_frequency(int freq);
	void start_timer();
	void stop_timer();
	bool is_running();

	void send_message_to_vpi(const VBMessage& msg);
	VBMessage receive_message_to_vpi(); // blocking
	void send_message_to_gui(const VBMessage& msg);
	VBMessage receive_message_to_gui(); // non blocking

private:
	int  gui_thread_func();
	void stop_gui_thread();
	void create_timer();
};


#endif /* VIRTUAL_BOARD_HH */
