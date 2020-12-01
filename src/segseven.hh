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

#ifndef ELNATH_SIMULATOR_SEGSEVEN_HH
#define ELNATH_SIMULATOR_SEGSEVEN_HH

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>

class SegSeven : public Gtk::DrawingArea {
public:
	typedef enum {
		SEGSEVEN_ALONE = 0,
		SEGSEVEN_LEFT,
		SEGSEVEN_MIDDLE,
		SEGSEVEN_RIGHT
	} position_t;

	SegSeven(position_t = SEGSEVEN_ALONE);
	virtual ~SegSeven();
	unsigned char get_state();
	unsigned char set_state(unsigned char state);
	void set_digit(unsigned char digit, bool with_dot = false);

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

	unsigned char m_state;

private:
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_base;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_a;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_b;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_c;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_d;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_e;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_f;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_g;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_p;
};

#endif /* ELNATH_SIMULATOR_SEGSEVEN_HH */
