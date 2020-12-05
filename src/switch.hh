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

#ifndef ELNATH_SIMULATOR_SWITCH_HH
#define ELNATH_SIMULATOR_SWITCH_HH

#include <gtkmm/drawingarea.h>
#include <gtkmm/statusbar.h>
#include <gdkmm/pixbuf.h>

class Switch : public Gtk::DrawingArea {
public:
	static const guint statusbar_context_id = 42;
	typedef void(*switch_callbackfun)(const Switch *sw, bool state, void *arg);

	Switch();
	virtual ~Switch();
	bool get_state();
	bool toggle();
	void set_callback_function(switch_callbackfun fun, void *arg);
	void set_statusbar(Gtk::Statusbar& sb, int switchnum);

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
	bool on_enter_notify_event(const GdkEventCrossing *crossing_event);
	bool on_leave_notify_event(const GdkEventCrossing *crossing_event);
	bool on_motion_notify_event(const GdkEventMotion *motion_event);
	bool on_button_press_event(const GdkEventButton* button_event);
	bool on_button_release_event(const GdkEventButton* button_event);
	bool on_scroll_event(const GdkEventScroll *scroll_event);

	bool m_state;
	bool m_hoovered;
	bool m_button_pressed_on_target;
	void *m_cbarg;
	switch_callbackfun m_cbfun;
	Gtk::Statusbar *m_statusbar;
	int             m_switch_number;

private:
	void manage_hoovered(double x, double y);
	void statusbar_update();
	bool get_pointer_is_on_switch(double x, double y);
	void exec_callback();

	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_off;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_off_s;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_on;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_on_s;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;
};

#endif /* ELNATH_SIMULATOR_SWITCH_HH */
