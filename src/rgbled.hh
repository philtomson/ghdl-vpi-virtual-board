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

#ifndef ELNATH_SIMULATOR_RGB_LED_HH
#define ELNATH_SIMULATOR_RGB_LED_HH

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>

class RGBLed : public Gtk::DrawingArea {
public:
	RGBLed();
	virtual ~RGBLed();

	int set_state(int state);

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

	int m_state;

private:
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_off;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_1;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_2;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_3;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_4;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_5;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_6;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_7;
};

#endif /* ELNATH_SIMULATOR_RGB_LED_HH */
