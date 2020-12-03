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

#ifndef INSPECTOR_WINDOW_HH
#define INSPECTOR_WINDOW_HH

#include <gtkmm/window.h>
#include <gtkmm/headerbar.h>

class VirtualBoard;


class InspectorWindow : public Gtk::Window {
private:
	VirtualBoard  *m_virtual_board;
	Gtk::HeaderBar m_headerbar;

public:
	InspectorWindow(VirtualBoard *vb);

private:
	bool on_my_delete_event(GdkEventAny* any_event);
};


#endif /* INSPECTOR_WINDOW_HH */
