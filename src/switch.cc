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

#include "switch.hh"
#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <iostream>


Switch::Switch() :
	m_state(false),
	m_hoovered(false),
	m_button_pressed_on_target(false),
	m_cbarg(0),
	m_cbfun(0),
	m_statusbar(nullptr),
	m_switch_number(0)
{
	m_pixbuf_off   = Gdk::Pixbuf::create_from_resource("/images/switch_off.png");
	m_pixbuf_off_s = Gdk::Pixbuf::create_from_resource("/images/switch_off_s.png");
	m_pixbuf_on    = Gdk::Pixbuf::create_from_resource("/images/switch_on.png");
	m_pixbuf_on_s  = Gdk::Pixbuf::create_from_resource("/images/switch_on_s.png");
	m_pixbuf = m_pixbuf_off;

	set_size_request(m_pixbuf->get_width(), m_pixbuf->get_height());

	signal_button_press_event().connect(sigc::mem_fun(*this, &Switch::on_button_press_event));
	signal_button_release_event().connect(sigc::mem_fun(*this, &Switch::on_button_release_event));
	signal_scroll_event().connect(sigc::mem_fun(*this, &Switch::on_scroll_event));
	signal_motion_notify_event().connect(sigc::mem_fun(*this, &Switch::on_motion_notify_event));
	signal_enter_notify_event().connect(sigc::mem_fun(*this, &Switch::on_enter_notify_event));
	signal_leave_notify_event().connect(sigc::mem_fun(*this, &Switch::on_leave_notify_event));

	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK | Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
}


Switch::~Switch()
{
}


void Switch::set_statusbar(Gtk::Statusbar& sb, int switchnum)
{
	m_statusbar = &sb;
	m_switch_number = switchnum;
}


bool Switch::get_state()
{
	return m_state;
}


void Switch::set_callback_function(Switch::switch_callbackfun fun, void *arg)
{
	m_cbarg = arg;
	m_cbfun = fun;
}


void Switch::exec_callback()
{
	if (m_cbfun)
		m_cbfun(this, m_state, m_cbarg);
}


bool Switch::get_pointer_is_on_switch(double x, double y)
{
	Gtk::Allocation allocation = get_allocation();
	const double px = x - ((allocation.get_width()  - m_pixbuf->get_width()) /2) - 2.0;
	const double py = y - ((allocation.get_height() - m_pixbuf->get_height())/2) - 2.0;
	bool res;

	if (px >= 0.0 && px < 26 && py >= 0.0 && py < 51)
		res = true;
	else
		res = false;

	return res;
}


void Switch::manage_hoovered(double x, double y)
{
	bool hoovered = get_pointer_is_on_switch(x, y);

	if (hoovered != m_hoovered) {
		m_hoovered = hoovered;
		if (m_state) {
			if (m_hoovered)
				m_pixbuf = m_pixbuf_on_s;
			else
				m_pixbuf = m_pixbuf_on;
		}
		else {
			if (m_hoovered)
				m_pixbuf = m_pixbuf_off_s;
			else
				m_pixbuf = m_pixbuf_off;
		}
		queue_draw();
		statusbar_update();
	}
}


bool Switch::on_button_press_event(const GdkEventButton* button_event)
{
//	switch (button_event->type) {
//		case GDK_BUTTON_PRESS:
//			std::cout << "GDK_BUTTON_PRESS" << std::endl;
//			break;
//		case GDK_2BUTTON_PRESS:
//			std::cout << "GDK_2BUTTON_PRESS" << std::endl;
//			break;
//		case GDK_3BUTTON_PRESS:
//			std::cout << "GDK_3BUTTON_PRESS" << std::endl;
//			break;
//		case GDK_BUTTON_RELEASE:
//			std::cout << "GDK_3BUTTON_PRESS" << std::endl;
//			break;
//	}
	//m_button_pressed_on_target = (button_event->type == GDK_BUTTON_PRESS && button_event->button == 1 && get_pointer_is_on_switch(button_event->x, button_event->y));
	m_button_pressed_on_target = (button_event->button == 1 && get_pointer_is_on_switch(button_event->x, button_event->y));
	return true;
}


bool Switch::on_button_release_event(const GdkEventButton* button_event)
{
	if (m_button_pressed_on_target && get_pointer_is_on_switch(button_event->x, button_event->y)) {
		m_state = !m_state;
		if (m_state) {
			//std::cout << "ON" << std::endl;
			if (m_hoovered)
				m_pixbuf = m_pixbuf_on_s;
			else
				m_pixbuf = m_pixbuf_on;
		}
		else {
			//std::cout << "OFF" << std::endl;
			if (m_hoovered)
				m_pixbuf = m_pixbuf_off_s;
			else
				m_pixbuf = m_pixbuf_off;
		}
		queue_draw();
		exec_callback();
		statusbar_update();
	}
	m_button_pressed_on_target = false;

	return true;
}


bool Switch::on_enter_notify_event(const GdkEventCrossing *crossing_event)
{
	manage_hoovered(crossing_event->x, crossing_event->y);
	return true;
}


bool Switch::on_leave_notify_event(const GdkEventCrossing *crossing_event)
{
	manage_hoovered(crossing_event->x, crossing_event->y);
	return true;
}


bool Switch::on_motion_notify_event(const GdkEventMotion *motion_event)
{
	manage_hoovered(motion_event->x, motion_event->y);
	return true;
}


bool Switch::on_scroll_event(const GdkEventScroll *scroll_event)
{
	m_button_pressed_on_target = false;

	if (get_pointer_is_on_switch(scroll_event->x, scroll_event->y)) {
		if (!m_state && scroll_event->direction == GDK_SCROLL_UP) {
			m_state = true;
			//std::cout << "ON" << std::endl;
			if (m_hoovered)
				m_pixbuf = m_pixbuf_on_s;
			else
				m_pixbuf = m_pixbuf_on;
			queue_draw();
			exec_callback();
			statusbar_update();
		}
		else if (m_state && scroll_event->direction == GDK_SCROLL_DOWN) {
			m_state = false;
			//std::cout << "OFF" << std::endl;
			if (m_hoovered)
				m_pixbuf = m_pixbuf_off_s;
			else
				m_pixbuf = m_pixbuf_off;
			queue_draw();
			exec_callback();
			statusbar_update();
		}
	}

	return true;
}


bool Switch::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
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


void Switch::statusbar_update()
{
	if (!m_statusbar)
		return;

	m_statusbar->pop(Switch::statusbar_context_id);
	if (m_hoovered)
		m_statusbar->push(std::string("Switch ") + std::to_string(m_switch_number) + std::string(" ") + (m_state ? std::string("ON") : std::string("OFF")), Switch::statusbar_context_id);
	//else
	//	m_statusbar->remove_all_messages(Switch::statusbar_context_id);
	//	//m_statusbar->pop(Switch::statusbar_context_id);
}


