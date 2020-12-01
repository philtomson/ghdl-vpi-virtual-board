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

#ifndef ELNATH_SIMULATOR_Led_HH
#define ELNATH_SIMULATOR_Led_HH

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>

class Led : public Gtk::DrawingArea {
public:
	Led();
	virtual ~Led();

	bool set_state(bool state);
	bool set_on();
	bool set_off();

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

	bool m_state;

private:
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_off;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_on;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;
};

#endif /* ELNATH_SIMULATOR_Led_HH */
