#include <gtkmm/application.h>
#include "board_window.hh"

int main()
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("fr.ensta_bretagne.bollengier.vpi_virtual_board", Gio::APPLICATION_NON_UNIQUE);
	VBWindow window;
	return app->run(window);
}

