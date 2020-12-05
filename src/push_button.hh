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

#ifndef ELNATH_SIMULATOR_PUSH_BUTTON_HH
#define ELNATH_SIMULATOR_PUSH_BUTTON_HH

#include <gtkmm/drawingarea.h>
#include <gtkmm/statusbar.h>
#include <gdkmm/pixbuf.h>

class PushButton : public Gtk::DrawingArea {
public:
	static const guint statusbar_context_id = 43;
	typedef void(*push_button_callbackfun)(const PushButton *pb, bool state, void *arg);
	typedef enum {
		PUSH_BUTTON_PADS_HORIZONTAL = 0,
		PUSH_BUTTON_PADS_VERTICAL
	} push_button_pads_orientation_t;

	PushButton(push_button_pads_orientation_t pads_orientation = PUSH_BUTTON_PADS_HORIZONTAL, bool active_high = true);
	virtual ~PushButton();
	bool get_state();
	bool set_state(bool state);
	void set_callback_function(push_button_callbackfun fun, void *arg);
	void set_statusbar(Gtk::Statusbar& sb, std::string const& button_name);


protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
	bool on_enter_notify_event(const GdkEventCrossing *crossing_event);
	bool on_leave_notify_event(const GdkEventCrossing *crossing_event);
	bool on_motion_notify_event(const GdkEventMotion *motion_event);
	bool on_button_press_event(const GdkEventButton* button_event);
	bool on_button_release_event(const GdkEventButton* button_event);
	bool on_scroll_event(const GdkEventScroll *scroll_event);
	void on_timeout_event();

	bool  m_active_low;
	bool  m_state;
	bool  m_hoovered;
	void *m_cbarg;
	push_button_callbackfun m_cbfun;
	Gtk::Statusbar *m_statusbar;
	std::string     m_button_name;

private:
	void manage_hoovered(double x, double y);
	bool get_pointer_is_on_button(double x, double y);
	void exec_callback();
	void statusbar_update();

	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_pads;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_off;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_off_s;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_on;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_on_s;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;
};

#endif /* ELNATH_SIMULATOR_PUSH_BUTTON_HH */
