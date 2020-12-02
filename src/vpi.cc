#include <cstdlib>
#include <cstdio>
#include <vpi_user.h>
#include "virtual_board.hh"


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
				if (strcmp("clk", net_name) == 0 && net_width == 1)
					vboard.clk_net = net;
				else if (strcmp("rstn", net_name) == 0 && net_width == 1)
					vboard.rstn_net = net;
				else if (strcmp("switches", net_name) == 0 && net_width == 16)
					vboard.switches_net = net;
				else if (strcmp("button_c", net_name) == 0 && net_width == 1)
					vboard.button_c_net = net;
				else if (strcmp("button_u", net_name) == 0 && net_width == 1)
					vboard.button_u_net = net;
				else if (strcmp("button_d", net_name) == 0 && net_width == 1)
					vboard.button_d_net = net;
				else if (strcmp("button_r", net_name) == 0 && net_width == 1)
					vboard.button_r_net = net;
				else if (strcmp("button_l", net_name) == 0 && net_width == 1)
					vboard.button_l_net = net;
				else
					vpi_free_object(net);
			}
			else if (net_dir == vpiOutput) {
				if (strcmp("leds", net_name) == 0 && net_width == 16)
					vboard.leds_net = net;
				else if (strcmp("display0", net_name) == 0 && net_width == 8)
					vboard.display0_net = net;
				else if (strcmp("display1", net_name) == 0 && net_width == 8)
					vboard.display1_net = net;
				else if (strcmp("display2", net_name) == 0 && net_width == 8)
					vboard.display2_net = net;
				else if (strcmp("display3", net_name) == 0 && net_width == 8)
					vboard.display3_net = net;
				else if (strcmp("display4", net_name) == 0 && net_width == 8)
					vboard.display4_net = net;
				else if (strcmp("display5", net_name) == 0 && net_width == 8)
					vboard.display5_net = net;
				else if (strcmp("display6", net_name) == 0 && net_width == 8)
					vboard.display6_net = net;
				else if (strcmp("display7", net_name) == 0 && net_width == 8)
					vboard.display7_net = net;
				else if (strcmp("rgb0", net_name) == 0 && net_width == 3)
					vboard.rgb0_net = net;
				else if (strcmp("rgb1", net_name) == 0 && net_width == 3)
					vboard.rgb1_net = net;
				else
					vpi_free_object(net);
			}
			else
				vpi_free_object(net);
		}
	}
	else
		vpi_free_object(iter);

	vpi_free_object(top);
}


static PLI_INT32 start_of_sim_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;

	vpi_printf("Start of simulation\n");

	gather_toplevel_IO_nets(*vboard);

	if (!vboard->clk_net) {
		vpi_printf("ERROR: could not find and input net named \"clk\" of 1 bit!\n");
		return 1;
	}
	else
		vpi_printf("Found input net clk[1]\n");
	if (vboard->rstn_net)
		vpi_printf("Found input net rstn[1]\n");
	else
		vpi_printf("Input net rstn[1] NOT FOUND\n");
	if (vboard->switches_net)
		vpi_printf("Found input net switches[16]\n");
	else
		vpi_printf("Input net switches[16] NOT FOUND\n");
	if (vboard->button_c_net)
		vpi_printf("Found input net button_c[1]\n");
	else
		vpi_printf("Input net button_c[1] NOT FOUND\n");
	if (vboard->button_u_net)
		vpi_printf("Found input net button_u[1]\n");
	else
		vpi_printf("Input net button_u[1] NOT FOUND\n");
	if (vboard->button_d_net)
		vpi_printf("Found input net button_d[1]\n");
	else
		vpi_printf("Input net button_d[1] NOT FOUND\n");
	if (vboard->button_r_net)
		vpi_printf("Found input net button_r[1]\n");
	else
		vpi_printf("Input net button_r[1] NOT FOUND\n");
	if (vboard->button_l_net)
		vpi_printf("Found input net button_l[1]\n");
	else
		vpi_printf("Input net button_l[1] NOT FOUND\n");
	if (vboard->leds_net)
		vpi_printf("Found output net leds[16]\n");
	else
		vpi_printf("Output net leds[16] NOT FOUND\n");
	if (vboard->display0_net)
		vpi_printf("Found output net display0[8]\n");
	else
		vpi_printf("Output net display0[8] NOT FOUND\n");
	if (vboard->display1_net)
		vpi_printf("Found output net display1[8]\n");
	else
		vpi_printf("Output net display1[8] NOT FOUND\n");
	if (vboard->display2_net)
		vpi_printf("Found output net display2[8]\n");
	else
		vpi_printf("Output net display2[8] NOT FOUND\n");
	if (vboard->display3_net)
		vpi_printf("Found output net display3[8]\n");
	else
		vpi_printf("Output net display3[8] NOT FOUND\n");
	if (vboard->display4_net)
		vpi_printf("Found output net display4[8]\n");
	else
		vpi_printf("Output net display4[8] NOT FOUND\n");
	if (vboard->display5_net)
		vpi_printf("Found output net display5[8]\n");
	else
		vpi_printf("Output net display5[8] NOT FOUND\n");
	if (vboard->display6_net)
		vpi_printf("Found output net display6[8]\n");
	else
		vpi_printf("Output net display6[8] NOT FOUND\n");
	if (vboard->display7_net)
		vpi_printf("Found output net display7[8]\n");
	else
		vpi_printf("Output net display7[8] NOT FOUND\n");
	if (vboard->rgb0_net)
		vpi_printf("Found output net rgb0[3]\n");
	else
		vpi_printf("Output net rgb0[3] NOT FOUND\n");
	if (vboard->rgb1_net)
		vpi_printf("Found output net rgb1[3]\n");
	else
		vpi_printf("Output net rgb1[3] NOT FOUND\n");

	vboard->start_gui_thread();

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
	if (!callback_handle)
		vpi_printf("ERROR: Cannot register cbStartOfSimulation call back\n");
	vpi_free_object(callback_handle);

	/* Register end of simulation callback */
	cb.reason = cbEndOfSimulation;
	cb.cb_rtn = &end_of_sim_cb;
	cb.user_data = (PLI_BYTE8*)vboard;

	callback_handle = vpi_register_cb(&cb);
	if (callback_handle == NULL)
		vpi_printf("ERROR: Cannot register cbEndOfSimulation call back\n");
	vpi_free_object(callback_handle);
}


/* Tell the simulator what is the entry point of our VPI application */
void (*vlog_startup_routines[]) () = {entry_point_cb, 0};

