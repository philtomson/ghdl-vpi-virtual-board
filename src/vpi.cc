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

#include <cstdlib>
#include <cstdio>
#include <vpi_user.h>
#include "virtual_board.hh"


static void register_cb_after(PLI_INT32 (*cb_rtn)(struct t_cb_data *), double delay, VirtualBoard *vboard)
{
	s_cb_data cb;
	vpiHandle callback_handle;
	s_vpi_time time = vboard->get_time(delay);

	cb.reason = cbAfterDelay;
	cb.cb_rtn = cb_rtn;
	cb.obj = NULL;
	cb.time = &time;
	cb.value = NULL;
	cb.user_data = (PLI_BYTE8*)vboard;

	callback_handle = vpi_register_cb(&cb);
	if (!callback_handle) {
		vpi_printf("\e[31mERROR: Cannot register cbAfterDelay\e[0m\n");
		exit(EXIT_FAILURE);
	}
	vpi_free_object(callback_handle);
}


static void register_cb_at_last_known_delta(PLI_INT32 (*cb_rtn)(struct t_cb_data *), VirtualBoard *vboard)
{
	s_cb_data cb;
	vpiHandle callback_handle;
	s_vpi_time time = {.type = vpiSimTime, .high = 0, .low = 0, .real = 0.0};

	cb.reason = cbReadWriteSynch;
	cb.cb_rtn = cb_rtn;
	cb.obj = NULL;
	cb.time = &time;
	cb.value = NULL;
	cb.user_data = (PLI_BYTE8*)vboard;

	callback_handle = vpi_register_cb(&cb);
	if (!callback_handle)
		vpi_printf("\e[31mERROR: Cannot register cbReadWriteSynch callback!\e[0m\n");
	vpi_free_object(callback_handle);
}


static void register_value_change_cb(PLI_INT32 (*cb_rtn)(struct t_cb_data *), vpiHandle net, VirtualBoard *vboard)
{
	s_cb_data cb;
	s_vpi_time tim;
	s_vpi_value val;
	vpiHandle callback_handle;

	tim.type = vpiSuppressTime;
	val.format = vpiSuppressVal;

	cb.reason = cbValueChange;
	cb.cb_rtn = cb_rtn;
	cb.obj    = net;
	cb.time   = &tim;
	cb.value  = &val;
	cb.user_data = (PLI_BYTE8*)vboard;

	callback_handle = vpi_register_cb(&cb);
	if (!callback_handle)
		vpi_printf ("\e[31mERROR: Cannot register cbValueChange call back\e[0m\n");
	vpi_free_object(callback_handle);
}


static void put_integer_value_to_net(vpiHandle net, int value)
{
	if (!net)
		return;
	s_vpi_value val;
	val.format = vpiIntVal;
	val.value.integer = value;
	vpi_put_value(net, &val, NULL, vpiNoDelay);
}


static int get_integer_value_from_net(vpiHandle net)
{
	int ret = 0, i, w = vpi_get(vpiSize, net);
	s_vpi_value val;
	val.format = vpiBinStrVal;
	vpi_get_value(net, &val);
	for (i = 0; i < w; i++) {
		ret <<= 1;
		if (val.value.str[i] == '1')
			ret |= 1;
	}

	return ret;
}


static void gather_toplevel_IO_nets(VirtualBoard& vboard)
{
	vpiHandle iter, top, net;
	const char *net_name;
	int         net_width;
	int         net_dir;

	iter = vpi_iterate(vpiModule, NULL);
	top = vpi_scan(iter);
	vpi_free_object(iter);

	vboard.design_top_unit_name = std::string(vpi_get_str(vpiName, top));

	iter = vpi_iterate(vpiNet, top);
	if (iter) {
		while ((net = vpi_scan(iter))) {
			net_name = vpi_get_str(vpiName, net);
			net_width = vpi_get(vpiSize, net);
			net_dir = vpi_get(vpiDirection, net);
			//vpi_printf("Net %s[%d] %s\n", net_name, net_width,
			//		(net_dir == vpiInput) ? "Input" : ((net_dir == vpiOutput) ? "Output" : ((net_dir == vpiInout) ? "Inout" : "")));
			if (net_dir == vpiInput) {
				if (strcmp("clk", net_name) == 0 && net_width == 1) {
					vboard.clk_net = net;
					vpi_printf("\e[32mFound input net clk[1]\e[0m\n");
				}
				else if (strcmp("rstn", net_name) == 0 && net_width == 1) {
					vboard.rstn_net = net;
					vpi_printf("\e[32mFound input net rstn[1]\e[0m\n");
				}
				else if (strcmp("switches", net_name) == 0 && net_width == 16) {
					vboard.switches_net = net;
					vpi_printf("\e[32mFound input net switches[16]\e[0m\n");
				}
				else if (strcmp("button_c", net_name) == 0 && net_width == 1) {
					vboard.button_c_net = net;
					vpi_printf("\e[32mFound input net button_c[1]\e[0m\n");
				}
				else if (strcmp("button_u", net_name) == 0 && net_width == 1) {
					vboard.button_u_net = net;
					vpi_printf("\e[32mFound input net button_u[1]\e[0m\n");
				}
				else if (strcmp("button_d", net_name) == 0 && net_width == 1) {
					vboard.button_d_net = net;
					vpi_printf("\e[32mFound input net button_d[1]\e[0m\n");
				}
				else if (strcmp("button_r", net_name) == 0 && net_width == 1) {
					vboard.button_r_net = net;
					vpi_printf("\e[32mFound input net button_r[1]\e[0m\n");
				}
				else if (strcmp("button_l", net_name) == 0 && net_width == 1) {
					vboard.button_l_net = net;
					vpi_printf("\e[32mFound input net button_l[1]\e[0m\n");
				}
				else {
					vpi_printf("\e[33mIgnoring input net %s[%d]\e[0m\n", net_name, net_width);
					vpi_free_object(net);
				}
			}
			else if (net_dir == vpiOutput) {
				if (strcmp("leds", net_name) == 0 && net_width == 16) {
					vboard.leds_net = net;
					vpi_printf("\e[32mFound output net leds[16]\e[0m\n");
				}
				else if (strcmp("display0", net_name) == 0 && net_width == 8) {
					vboard.display0_net = net;
					vpi_printf("\e[32mFound output net display0[8]\e[0m\n");
				}
				else if (strcmp("display1", net_name) == 0 && net_width == 8) {
					vboard.display1_net = net;
					vpi_printf("\e[32mFound output net display1[8]\e[0m\n");
				}
				else if (strcmp("display2", net_name) == 0 && net_width == 8) {
					vboard.display2_net = net;
					vpi_printf("\e[32mFound output net display2[8]\e[0m\n");
				}
				else if (strcmp("display3", net_name) == 0 && net_width == 8) {
					vboard.display3_net = net;
					vpi_printf("\e[32mFound output net display3[8]\e[0m\n");
				}
				else if (strcmp("display4", net_name) == 0 && net_width == 8) {
					vboard.display4_net = net;
					vpi_printf("\e[32mFound output net display4[8]\e[0m\n");
				}
				else if (strcmp("display5", net_name) == 0 && net_width == 8) {
					vboard.display5_net = net;
					vpi_printf("\e[32mFound output net display5[8]\e[0m\n");
				}
				else if (strcmp("display6", net_name) == 0 && net_width == 8) {
					vboard.display6_net = net;
					vpi_printf("\e[32mFound output net display6[8]\e[0m\n");
				}
				else if (strcmp("display7", net_name) == 0 && net_width == 8) {
					vboard.display7_net = net;
					vpi_printf("\e[32mFound output net display7[8]\e[0m\n");
				}
				else if (strcmp("rgb0", net_name) == 0 && net_width == 3) {
					vboard.rgb0_net = net;
					vpi_printf("\e[32mFound output net rgb0[3]\e[0m\n");
				}
				else if (strcmp("rgb1", net_name) == 0 && net_width == 3) {
					vboard.rgb1_net = net;
					vpi_printf("\e[32mFound output net rgb1[3]\e[0m\n");
				}
				else {
					vpi_printf("\e[33mIgnoring output net %s[%d]\e[0m\n", net_name, net_width);
					vpi_free_object(net);
				}
			}
			else
				vpi_free_object(net);
		}
	}
	else
		vpi_free_object(iter);

	vpi_free_object(top);
}


static PLI_INT32 on_leds_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->leds_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::LEDS, v));
	return 0;
}


static PLI_INT32 on_display0_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->display0_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::DISPLAY_0, v));
	return 0;
}


static PLI_INT32 on_display1_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->display1_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::DISPLAY_1, v));
	return 0;
}


static PLI_INT32 on_display2_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->display2_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::DISPLAY_2, v));
	return 0;
}


static PLI_INT32 on_display3_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->display3_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::DISPLAY_3, v));
	return 0;
}


static PLI_INT32 on_display4_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->display4_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::DISPLAY_4, v));
	return 0;
}


static PLI_INT32 on_display5_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->display5_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::DISPLAY_5, v));
	return 0;
}


static PLI_INT32 on_display6_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->display6_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::DISPLAY_6, v));
	return 0;
}


static PLI_INT32 on_display7_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->display7_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::DISPLAY_7, v));
	return 0;
}


static PLI_INT32 on_rgb0_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->rgb0_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::RGB_LED_0, v));
	return 0;
}


static PLI_INT32 on_rgb1_value_change(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	int v = get_integer_value_from_net(vboard->rgb1_net);
	vboard->send_message_to_gui(VBMessage::io_changed(VBMessage::RGB_LED_1, v));
	return 0;
}


static PLI_INT32 set_clock_callback(p_cb_data cb_data);
static PLI_INT32 reset_clock_callback(p_cb_data cb_data);


static PLI_INT32 main_callback(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	bool advance_time = false;
	bool exit_sim = false;
	bool wait_other_messages = true;

	while (wait_other_messages) {
		VBMessage msg = vboard->receive_message_to_vpi();
		switch (msg.type()) {
			case VBMessage::MSG_CLOCK:
				if (vboard->is_running()) {
					if (vboard->cycles_to_go) {
						vboard->cycles_to_go--;
						if (vboard->cycles_to_go == 0) {
							vboard->stop_timer();
							vboard->send_message_to_gui(VBMessage::stopped());
						}
						wait_other_messages = false;
						advance_time = true;
					}
				}
				break;
			case VBMessage::MSG_IO_CHANGED:
				wait_other_messages = false;
				switch (msg.io_name()) {
					case VBMessage::SWITCHES:
						put_integer_value_to_net(vboard->switches_net, msg.value());
						break;
					case VBMessage::PUSH_BUTTON_RSTN:
						put_integer_value_to_net(vboard->rstn_net, msg.value());
						break;
					case VBMessage::PUSH_BUTTON_CENTER:
						put_integer_value_to_net(vboard->button_c_net, msg.value());
						break;
					case VBMessage::PUSH_BUTTON_UP:
						put_integer_value_to_net(vboard->button_u_net, msg.value());
						break;
					case VBMessage::PUSH_BUTTON_DOWN:
						put_integer_value_to_net(vboard->button_d_net, msg.value());
						break;
					case VBMessage::PUSH_BUTTON_RIGHT:
						put_integer_value_to_net(vboard->button_r_net, msg.value());
						break;
					case VBMessage::PUSH_BUTTON_LEFT:
						put_integer_value_to_net(vboard->button_l_net, msg.value());
						break;
					default:
						wait_other_messages = true;
				}
				break;
			case VBMessage::MSG_NONE:
				break;
			case VBMessage::MSG_EXIT:
				wait_other_messages = false;
				exit_sim = true;
				break;
			case VBMessage::MSG_RUN:
				vboard->cycles_to_go = -1;
				vboard->start_timer();
				vboard->send_message_to_gui(VBMessage::running());
				break;
			case VBMessage::MSG_RUN_N:
				/* run the first cycle now without waiting for the first timer notification */
				if (msg.value() <= 1) {
					vboard->cycles_to_go = 0;
				}
				else {
					vboard->cycles_to_go = msg.value() - 1;
					vboard->start_timer();
					vboard->send_message_to_gui(VBMessage::running());
				}
				wait_other_messages = false;
				advance_time = true;
				break;
			case VBMessage::MSG_STOP:
				vboard->cycles_to_go = 0;
				vboard->stop_timer();
				vboard->send_message_to_gui(VBMessage::stopped());
				break;
			case VBMessage::MSG_SET_FREQ:
				vboard->set_timer_frequency(msg.value());
				break;
			case VBMessage::MSG_UPDATE_SIGNALS:
				vboard->send_message_to_gui(VBMessage::signals_updated());
				break;
			default:
				printf("\e[31mVPI: Bad MSG: \"%s\" (%d)\e[0m\n", msg.type_to_s(), msg.type());
		}
	}
	
	if (!exit_sim) {
		if (advance_time)
			register_cb_after(set_clock_callback, vboard->half_period(), vboard);
		else
			register_cb_at_last_known_delta(main_callback, vboard);
	}

	return 0;
}


static PLI_INT32 set_clock_callback(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	put_integer_value_to_net(vboard->clk_net, 1);
	register_cb_after(reset_clock_callback, vboard->half_period(), vboard);
	return 0;
}


static PLI_INT32 reset_clock_callback(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	put_integer_value_to_net(vboard->clk_net, 0);
	register_cb_at_last_known_delta(main_callback, vboard);
	return 0;
}


static PLI_INT32 reset_startup_callback(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	static int startup_cnt = 0;
	VBMessage msg;

	switch (startup_cnt) {
		/* set all inputs to 0 */
		case 0:
			do {
				msg = vboard->receive_message_to_vpi();
			} while (msg.type() != VBMessage::MSG_GUI_STARTED);

			if (vboard->leds_net)
				register_value_change_cb(on_leds_value_change, vboard->leds_net, vboard);
			if (vboard->display0_net)
				register_value_change_cb(on_display0_value_change, vboard->display0_net, vboard);
			if (vboard->display1_net)
				register_value_change_cb(on_display1_value_change, vboard->display1_net, vboard);
			if (vboard->display2_net)
				register_value_change_cb(on_display2_value_change, vboard->display2_net, vboard);
			if (vboard->display3_net)
				register_value_change_cb(on_display3_value_change, vboard->display3_net, vboard);
			if (vboard->display4_net)
				register_value_change_cb(on_display4_value_change, vboard->display4_net, vboard);
			if (vboard->display5_net)
				register_value_change_cb(on_display5_value_change, vboard->display5_net, vboard);
			if (vboard->display6_net)
				register_value_change_cb(on_display6_value_change, vboard->display6_net, vboard);
			if (vboard->display7_net)
				register_value_change_cb(on_display7_value_change, vboard->display7_net, vboard);
			if (vboard->rgb0_net)
				register_value_change_cb(on_rgb0_value_change, vboard->rgb0_net, vboard);
			if (vboard->rgb1_net)
				register_value_change_cb(on_rgb1_value_change, vboard->rgb1_net, vboard);

			put_integer_value_to_net(vboard->clk_net, 0);
			put_integer_value_to_net(vboard->rstn_net, 0);
			put_integer_value_to_net(vboard->switches_net, 0);
			put_integer_value_to_net(vboard->button_c_net, 0);
			put_integer_value_to_net(vboard->button_u_net, 0);
			put_integer_value_to_net(vboard->button_d_net, 0);
			put_integer_value_to_net(vboard->button_r_net, 0);
			put_integer_value_to_net(vboard->button_l_net, 0);
			register_cb_after(reset_startup_callback, 5e-9, vboard);
			break;

		/* set clock to 1*/
		case 1:
			put_integer_value_to_net(vboard->clk_net, 1);
			register_cb_after(reset_startup_callback, 5e-9, vboard);
			break;

		/* set clock to 0, rstn to 1, register main callback */
		case 2:
			put_integer_value_to_net(vboard->clk_net, 0);
			put_integer_value_to_net(vboard->rstn_net, 1);
			register_cb_after(reset_clock_callback, vboard->half_period() - 20e-9, vboard);
			break;

		default:
			vpi_printf("\e[31mERROR: should not enter this callback again!\3[0m\n");
	}

	startup_cnt++;

	return 0;
}


static PLI_INT32 start_of_sim_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;

	vpi_printf("Start of simulation\n");

	s_vpi_vlog_info info;
	vpi_get_vlog_info(&info);
	vboard->simulator_name = std::string(info.product);
	vboard->simulator_version = std::string(info.version);

	gather_toplevel_IO_nets(*vboard);

	if (!vboard->clk_net) {
		vpi_printf("\e[31mERROR: could not find an input net named \"clk\" of 1 bit!\e[0m\n");
		return 1;
	}
	if (!vboard->rstn_net)
		vpi_printf("\e[33mInput net rstn[1] NOT FOUND\e[0m\n");
	if (!vboard->switches_net)
		vpi_printf("\e[33mInput net switches[16] NOT FOUND\e[0m\n");
	if (!vboard->button_c_net)
		vpi_printf("\e[33mInput net button_c[1] NOT FOUND\e[0m\n");
	if (!vboard->button_u_net)
		vpi_printf("\e[33mInput net button_u[1] NOT FOUND\e[0m\n");
	if (!vboard->button_d_net)
		vpi_printf("\e[33mInput net button_d[1] NOT FOUND\e[0m\n");
	if (!vboard->button_r_net)
		vpi_printf("\e[33mInput net button_r[1] NOT FOUND\e[0m\n");
	if (!vboard->button_l_net)
		vpi_printf("\e[33mInput net button_l[1] NOT FOUND\e[0m\n");
	if (!vboard->leds_net)
		vpi_printf("\e[33mOutput net leds[16] NOT FOUND\e[0m\n");
	if (!vboard->display0_net)
		vpi_printf("\e[33mOutput net display0[8] NOT FOUND\e[0m\n");
	if (!vboard->display1_net)
		vpi_printf("\e[33mOutput net display1[8] NOT FOUND\e[0m\n");
	if (!vboard->display2_net)
		vpi_printf("\e[33mOutput net display2[8] NOT FOUND\e[0m\n");
	if (!vboard->display3_net)
		vpi_printf("\e[33mOutput net display3[8] NOT FOUND\e[0m\n");
	if (!vboard->display4_net)
		vpi_printf("\e[33mOutput net display4[8] NOT FOUND\e[0m\n");
	if (!vboard->display5_net)
		vpi_printf("\e[33mOutput net display5[8] NOT FOUND\e[0m\n");
	if (!vboard->display6_net)
		vpi_printf("\e[33mOutput net display6[8] NOT FOUND\e[0m\n");
	if (!vboard->display7_net)
		vpi_printf("\e[33mOutput net display7[8] NOT FOUND\e[0m\n");
	if (!vboard->rgb0_net)
		vpi_printf("\e[33mOutput net rgb0[3] NOT FOUND\e[0m\n");
	if (!vboard->rgb1_net)
		vpi_printf("\e[33mOutput net rgb1[3] NOT FOUND\e[0m\n");

	vboard->set_time_resolution(vpi_get(vpiTimePrecision, NULL));

	vboard->start_gui_thread();

	register_cb_after(reset_startup_callback, 5e-9, vboard);

	return 0;
}


static PLI_INT32 end_of_sim_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;

	vpi_printf("End of simulation\n");


	if (vboard) {
		vboard->send_message_to_gui(VBMessage::exit());
		delete vboard;
	}

	return 0;
}


static void entry_point_cb()
{
	VirtualBoard *vboard = new VirtualBoard();

	s_cb_data cb;
	vpiHandle callback_handle;

	/* Register start of simulation callback (just before time 0) */
	cb.reason = cbStartOfSimulation;
	cb.cb_rtn = &start_of_sim_cb;
	cb.user_data = (PLI_BYTE8*)vboard;

	callback_handle = vpi_register_cb(&cb);
	if (!callback_handle) {
		vpi_printf("\e[31mERROR: Cannot register cbStartOfSimulation call back\e[0m\n");
		exit(EXIT_FAILURE);
	}
	vpi_free_object(callback_handle);

	/* Register end of simulation callback */
	cb.reason = cbEndOfSimulation;
	cb.cb_rtn = &end_of_sim_cb;
	cb.user_data = (PLI_BYTE8*)vboard;

	callback_handle = vpi_register_cb(&cb);
	if (!callback_handle) {
		vpi_printf("\e[31mERROR: Cannot register cbEndOfSimulation call back\e[0m\n");
		exit(EXIT_FAILURE);
	}
	vpi_free_object(callback_handle);
}


/* Tell the simulator what is the entry point of our VPI application */
void (*vlog_startup_routines[]) () = {entry_point_cb, 0};

