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

#include "segseven.hh"
#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <iostream>

#define SEGSEV_PIXBUF_WIDTH  34
#define SEGSEV_PIXBUF_HEIGHT 50


SegSeven::SegSeven(SegSeven::position_t pos) :
	m_state(0)
{
	switch (pos) {
		case SEGSEVEN_LEFT:
			m_pixbuf_base = Gdk::Pixbuf::create_from_resource("/images/7seg_base_left.png");
			break;
		case SEGSEVEN_MIDDLE:
			m_pixbuf_base = Gdk::Pixbuf::create_from_resource("/images/7seg_base_middle.png");
			break;
		case SEGSEVEN_RIGHT:
			m_pixbuf_base = Gdk::Pixbuf::create_from_resource("/images/7seg_base_right.png");
			break;
		default:
			m_pixbuf_base = Gdk::Pixbuf::create_from_resource("/images/7seg_base.png");
	}
	m_pixbuf_a = Gdk::Pixbuf::create_from_resource("/images/7seg_a.png");
	m_pixbuf_b = Gdk::Pixbuf::create_from_resource("/images/7seg_b.png");
	m_pixbuf_c = Gdk::Pixbuf::create_from_resource("/images/7seg_c.png");
	m_pixbuf_d = Gdk::Pixbuf::create_from_resource("/images/7seg_d.png");
	m_pixbuf_e = Gdk::Pixbuf::create_from_resource("/images/7seg_e.png");
	m_pixbuf_f = Gdk::Pixbuf::create_from_resource("/images/7seg_f.png");
	m_pixbuf_g = Gdk::Pixbuf::create_from_resource("/images/7seg_g.png");
	m_pixbuf_p = Gdk::Pixbuf::create_from_resource("/images/7seg_p.png");

	set_size_request(SEGSEV_PIXBUF_WIDTH, SEGSEV_PIXBUF_HEIGHT);
}


SegSeven::~SegSeven()
{
}


unsigned char SegSeven::get_state()
{
	return m_state;
}


unsigned char SegSeven::set_state(unsigned char state)
{
	unsigned char old_state = m_state;

	if (state != m_state) {
		m_state = state;
		queue_draw();
	}

	return old_state;
}


bool SegSeven::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();
	const int wid = allocation.get_width();
	const int hei = allocation.get_height();

	if (!m_pixbuf_base)
		return false;

	Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_base, 
			(wid - SEGSEV_PIXBUF_WIDTH)/2,
			(hei - SEGSEV_PIXBUF_HEIGHT)/2);

	cr->paint();

	if ((m_state & 0x01) && m_pixbuf_a) {
		Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_a, 
				(wid - SEGSEV_PIXBUF_WIDTH)/2,
				(hei - SEGSEV_PIXBUF_HEIGHT)/2);
		cr->paint();
	}
	if ((m_state & 0x02) && m_pixbuf_b) {
		Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_b, 
				(wid - SEGSEV_PIXBUF_WIDTH)/2,
				(hei - SEGSEV_PIXBUF_HEIGHT)/2);
		cr->paint();
	}
	if ((m_state & 0x04) && m_pixbuf_c) {
		Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_c, 
				(wid - SEGSEV_PIXBUF_WIDTH)/2,
				(hei - SEGSEV_PIXBUF_HEIGHT)/2);
		cr->paint();
	}
	if ((m_state & 0x08) && m_pixbuf_d) {
		Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_d, 
				(wid - SEGSEV_PIXBUF_WIDTH)/2,
				(hei - SEGSEV_PIXBUF_HEIGHT)/2);
		cr->paint();
	}
	if ((m_state & 0x10) && m_pixbuf_e) {
		Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_e, 
				(wid - SEGSEV_PIXBUF_WIDTH)/2,
				(hei - SEGSEV_PIXBUF_HEIGHT)/2);
		cr->paint();
	}
	if ((m_state & 0x20) && m_pixbuf_f) {
		Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_f, 
				(wid - SEGSEV_PIXBUF_WIDTH)/2,
				(hei - SEGSEV_PIXBUF_HEIGHT)/2);
		cr->paint();
	}
	if ((m_state & 0x40) && m_pixbuf_g) {
		Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_g, 
				(wid - SEGSEV_PIXBUF_WIDTH)/2,
				(hei - SEGSEV_PIXBUF_HEIGHT)/2);
		cr->paint();
	}
	if ((m_state & 0x80) && m_pixbuf_p) {
		Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_p, 
				(wid - SEGSEV_PIXBUF_WIDTH)/2,
				(hei - SEGSEV_PIXBUF_HEIGHT)/2);
		cr->paint();
	}

	return true;
}


void SegSeven::set_digit(unsigned char digit, bool with_dot)
{
	static const unsigned char segdigits[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71};
	unsigned char state = segdigits[digit & 0x0f];
	if (with_dot)
		state |= 0x80;
	set_state(state);
}


