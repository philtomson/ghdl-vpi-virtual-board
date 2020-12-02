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
#include "board_window.hh"


class VirtualBoard {
private:
	std::thread *m_thread;
	VBWindow    *m_window;
public:
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
	vpiHandle    rgb0_net;
	vpiHandle    rgb1_net;

public:
	VirtualBoard();
	~VirtualBoard();

	void start_gui_thread();

private:
	int  gui_thread_func();
	void stop_gui_thread();
};


#endif /* VIRTUAL_BOARD_HH */
