#include "virtual_board.hh"

VirtualBoard::VirtualBoard() :
	m_thread(nullptr),
	m_window(nullptr)
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
	if (m_thread->joinable())
		m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}


