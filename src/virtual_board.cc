#include "virtual_board.hh"

VirtualBoard::VirtualBoard() :
	m_thread(nullptr),
	m_window(nullptr),
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
	m_window = new VBWindow();
	ret = app->run(*m_window);
	delete m_window;
	m_window = nullptr;
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


