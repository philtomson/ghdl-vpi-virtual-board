#include "InspectorWindow.hh"
#include "virtual_board.hh"


InspectorWindow::InspectorWindow(VirtualBoard* vb) :
	Gtk::Window(),
	m_virtual_board(vb),
	m_headerbar()
{
	set_icon_name("gtk-find");
	m_headerbar.set_title("Signal inspector");
	m_headerbar.set_show_close_button();
	set_titlebar(m_headerbar);
	signal_delete_event().connect(sigc::mem_fun(*this, &InspectorWindow::on_my_delete_event));
}


bool InspectorWindow::on_my_delete_event(GdkEventAny* any_event)
{
	(void)any_event;
	hide();
	return true;
}


