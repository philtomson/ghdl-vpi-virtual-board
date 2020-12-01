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

#include "rgbled.hh"
#include <cairomm/context.h>
#include <gdkmm/general.h>


RGBLed::RGBLed() :
	m_state(false)
{
	m_pixbuf_off   = Gdk::Pixbuf::create_from_resource("/images/rgb_0.png");
	m_pixbuf_1     = Gdk::Pixbuf::create_from_resource("/images/rgb_1.png");
	m_pixbuf_2     = Gdk::Pixbuf::create_from_resource("/images/rgb_2.png");
	m_pixbuf_3     = Gdk::Pixbuf::create_from_resource("/images/rgb_3.png");
	m_pixbuf_4     = Gdk::Pixbuf::create_from_resource("/images/rgb_4.png");
	m_pixbuf_5     = Gdk::Pixbuf::create_from_resource("/images/rgb_5.png");
	m_pixbuf_6     = Gdk::Pixbuf::create_from_resource("/images/rgb_6.png");
	m_pixbuf_7     = Gdk::Pixbuf::create_from_resource("/images/rgb_7.png");

	set_size_request(m_pixbuf_off->get_width(), m_pixbuf_off->get_height());
}


RGBLed::~RGBLed()
{
}


int RGBLed::set_state(int state)
{
	const int old_state = m_state;

	state &= 7;

	if (state != m_state) {
		m_state = state;
		queue_draw();
	}

	return old_state;
}


bool RGBLed::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();
	bool reprint = true;
	Glib::RefPtr<Gdk::Pixbuf> colorpix;

	Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_off, 
			(allocation.get_width() - m_pixbuf_off->get_width())/2,
			(allocation.get_height() - m_pixbuf_off->get_height())/2);

	cr->paint();

	switch (m_state) {
		case 1:
			colorpix = m_pixbuf_1;
			break;
		case 2:
			colorpix = m_pixbuf_2;
			break;
		case 3:
			colorpix = m_pixbuf_3;
			break;
		case 4:
			colorpix = m_pixbuf_4;
			break;
		case 5:
			colorpix = m_pixbuf_5;
			break;
		case 6:
			colorpix = m_pixbuf_6;
			break;
		case 7:
			colorpix = m_pixbuf_7;
			break;
		default:
			reprint = false;
	}

	if (reprint) {
		Gdk::Cairo::set_source_pixbuf(cr, colorpix, 
				(allocation.get_width() - colorpix->get_width())/2,
				(allocation.get_height() - colorpix->get_height())/2);

		cr->paint();
	}

	return true;
}


