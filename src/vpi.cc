#include <cstdlib>
#include <cstdio>
#include <vpi_user.h>
#include <unistd.h>
#include "virtual_board.hh"


PLI_INT32 start_of_sim_cb(p_cb_data cb_data)
{
	VirtualBoard *vboard = (VirtualBoard*)cb_data->user_data;

	vpi_printf("Start of simulation\n");
	vboard->start_gui_thread();
	usleep(1000000);

	return 0;
}


PLI_INT32 end_of_sim_cb(p_cb_data cb_data)
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

