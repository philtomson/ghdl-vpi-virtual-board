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

#include "led.hh"
#include <cairomm/context.h>
#include <gdkmm/general.h>


Led::Led() :
	m_state(false)
{
	m_pixbuf_off   = Gdk::Pixbuf::create_from_resource("/images/led_off.png");
	m_pixbuf_on    = Gdk::Pixbuf::create_from_resource("/images/led_on.png");
	m_pixbuf = m_pixbuf_off;

	set_size_request(m_pixbuf->get_width(), m_pixbuf->get_height());
}


Led::~Led()
{
}


bool Led::set_state(bool state)
{
	if (state)
		return set_on();

	return set_off();
}


bool Led::set_on()
{
	if (m_state)
		return true;

	m_state = true;
	m_pixbuf = m_pixbuf_on;
	queue_draw();

	return false;
}


bool Led::set_off()
{
	if (m_state) {
		m_state = false;
		m_pixbuf = m_pixbuf_off;
		queue_draw();
		return true;
	}

	return false;
}


bool Led::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();

	if (!m_pixbuf)
		return false;

	Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf, 
			(allocation.get_width() - m_pixbuf->get_width())/2,
			(allocation.get_height() - m_pixbuf->get_height())/2);

	cr->paint();

	return true;
}


