#include <cmath>
#include "virtual_board.hh"

VirtualBoard::VirtualBoard() :
	m_thread(nullptr),
	m_window(nullptr),
	m_time_resolution(1.0),
	m_to_vpi_mutex(),
	m_to_vpi_condvar(),
	m_to_vpi_queue(),
	m_to_gui_mutex(),
	m_to_gui_queue(),
	clk_net(nullptr),
	rstn_net(nullptr),
	switches_net(nullptr),
	button_c_net(nullptr),
	button_u_net(nullptr),
	button_d_net(nullptr),
	button_r_net(nullptr),
	button_l_net(nullptr),
	leds_net(nullptr),
	display0_net(nullptr),
	display1_net(nullptr),
	display2_net(nullptr),
	display3_net(nullptr),
	display4_net(nullptr),
	display5_net(nullptr),
	display6_net(nullptr),
	display7_net(nullptr),
	rgb0_net(nullptr),
	rgb1_net(nullptr)
{
}


VirtualBoard::~VirtualBoard()
{
	stop_gui_thread();
}


void VirtualBoard::start_gui_thread()
{
	if (m_thread)
		return;

	m_thread = new std::thread(&VirtualBoard::gui_thread_func, this);
}


int VirtualBoard::gui_thread_func()
{
	int ret;

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("fr.ensta_bretagne.bollengier.vpi_virtual_board", Gio::APPLICATION_NON_UNIQUE);

	m_window = new VBWindow(this);

	ret = app->run(*m_window);

	VBWindow *tmp = m_window;
	m_window = nullptr;
	delete tmp;

	return ret;
}


void VirtualBoard::stop_gui_thread()
{
	if (!m_thread)
		return;
	if (m_thread->joinable()) {
		printf("Joining...\n");
		m_thread->join();
		printf("Joined\n");
	}
	delete m_thread;
	m_thread = nullptr;
}


void VirtualBoard::set_time_resolution(int res)
{
	m_time_resolution = std::pow(10, -res);
}


s_vpi_time VirtualBoard::get_time(double t)
{
	s_vpi_time ts;
	uint64_t simtime = (uint64_t)(t * m_time_resolution);

	ts.type = vpiSimTime;
	ts.low = (uint32_t)(simtime & 0xffffffffUL);
	ts.high = (uint32_t)(simtime >> 32);

	return ts;
}


void VirtualBoard::send_message_to_vpi(const VBMessage& msg)
{
	std::unique_lock<std::mutex> lock(m_to_vpi_mutex);
	m_to_vpi_queue.push(msg);
	m_to_vpi_condvar.notify_one();
}


VBMessage VirtualBoard::receive_message_to_vpi()
{
	VBMessage msg;
	std::unique_lock<std::mutex> lock(m_to_vpi_mutex);

	while (m_to_vpi_queue.empty())
		m_to_vpi_condvar.wait(lock);

	msg = m_to_vpi_queue.front();
	m_to_vpi_queue.pop();

	return msg;
}


void VirtualBoard::send_message_to_gui(const VBMessage& msg)
{
	if (!m_window)
		return;
	std::unique_lock<std::mutex> lock(m_to_gui_mutex);
	m_to_gui_queue.push(msg);
	m_window->notify_from_vpi();
}


VBMessage VirtualBoard::receive_message_to_gui()
{
	VBMessage msg;
	std::unique_lock<std::mutex> lock(m_to_vpi_mutex);

	if (m_to_gui_queue.empty())
		return msg;

	msg = m_to_vpi_queue.front();
	m_to_vpi_queue.pop();

	return msg;
}




