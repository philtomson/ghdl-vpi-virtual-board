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
	int ret = 0, i, j, w = vpi_get(vpiSize, net);
	s_vpi_value val;
	val.format = vpiBinStrVal;
	vpi_get_value(net, &val);
	for (i = 0, j = w - 1; i < w; i++, j--) {
		ret <<= 1;
		if (val.value.str[j] == '1')
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


/* set clock to 0 and rstn to 1 and register value change callbacks */
static PLI_INT32 at_fifteen_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;
	VBMessage msg;

	vpi_printf("at fifteen\n");
	
	put_integer_value_to_net(vboard->clk_net, 0);
	put_integer_value_to_net(vboard->rstn_net, 1);

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

	return 0;
}


/* set clock to 1 */
static PLI_INT32 at_ten_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;

	vpi_printf("at ten\n");

	put_integer_value_to_net(vboard->clk_net, 1);

	register_cb_after(at_fifteen_cb, 5e-9, vboard);

	return 0;
}


/* set all inputs to 0 */
static PLI_INT32 at_five_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;

	vpi_printf("at five\n");

	put_integer_value_to_net(vboard->clk_net, 0);
	put_integer_value_to_net(vboard->rstn_net, 0);
	put_integer_value_to_net(vboard->switches_net, 0);
	put_integer_value_to_net(vboard->button_c_net, 0);
	put_integer_value_to_net(vboard->button_u_net, 0);
	put_integer_value_to_net(vboard->button_d_net, 0);
	put_integer_value_to_net(vboard->button_r_net, 0);
	put_integer_value_to_net(vboard->button_l_net, 0);

	register_cb_after(at_ten_cb, 5e-9, vboard);

	return 0;
}


static PLI_INT32 start_of_sim_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;

	vpi_printf("Start of simulation\n");

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

	register_cb_after(at_five_cb, 5e-9, vboard);

	return 0;
}


static PLI_INT32 end_of_sim_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;

	vpi_printf("End of simulation\n");

	if (vboard)
		delete vboard;

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

