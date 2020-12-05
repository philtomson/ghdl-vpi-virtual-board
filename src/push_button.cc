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

#include "push_button.hh"
#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <glibmm/main.h>
#include <iostream>


PushButton::PushButton(push_button_pads_orientation_t pads_orientation, bool active_high) :
	m_active_low(!active_high),
	m_state(false),
	m_hoovered(false),
	m_cbarg(0),
	m_cbfun(0),
	m_statusbar(nullptr),
	m_button_name()
{
	if (pads_orientation == PushButton::PUSH_BUTTON_PADS_VERTICAL)
		m_pixbuf_pads = Gdk::Pixbuf::create_from_resource("/images/button_pads_v.png");
	else
		m_pixbuf_pads = Gdk::Pixbuf::create_from_resource("/images/button_pads_h.png");
	m_pixbuf_off   = Gdk::Pixbuf::create_from_resource("/images/button.png");
	m_pixbuf_off_s = Gdk::Pixbuf::create_from_resource("/images/button_s.png");
	m_pixbuf_on    = Gdk::Pixbuf::create_from_resource("/images/button_pressed.png");
	m_pixbuf_on_s  = Gdk::Pixbuf::create_from_resource("/images/button_pressed_s.png");
	m_pixbuf = m_pixbuf_off;

	set_size_request(m_pixbuf->get_width(), m_pixbuf->get_height());

	signal_button_press_event().connect(sigc::mem_fun(*this, &PushButton::on_button_press_event));
	signal_button_release_event().connect(sigc::mem_fun(*this, &PushButton::on_button_release_event));
	signal_scroll_event().connect(sigc::mem_fun(*this, &PushButton::on_scroll_event));
	signal_motion_notify_event().connect(sigc::mem_fun(*this, &PushButton::on_motion_notify_event));
	signal_enter_notify_event().connect(sigc::mem_fun(*this, &PushButton::on_enter_notify_event));
	signal_leave_notify_event().connect(sigc::mem_fun(*this, &PushButton::on_leave_notify_event));

	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK | Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
}


PushButton::~PushButton()
{
}


void PushButton::set_statusbar(Gtk::Statusbar& sb, std::string const& button_name)
{
	m_statusbar = &sb;
	m_button_name = std::string(button_name);
}


bool PushButton::get_state()
{
	return (m_state ^ m_active_low);
}


bool PushButton::set_state(bool state)
{
	if (state != m_state) {
		m_state = state;
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
		exec_callback();
		statusbar_update();
		return true;
	}

	return false;
}


void PushButton::set_callback_function(PushButton::push_button_callbackfun fun, void *arg)
{
	m_cbarg = arg;
	m_cbfun = fun;
}


void PushButton::exec_callback()
{
	if (m_cbfun)
		m_cbfun(this, m_state ^ m_active_low, m_cbarg);
}


bool PushButton::get_pointer_is_on_button(double x, double y)
{
	Gtk::Allocation allocation = get_allocation();
	const double px = x - ((allocation.get_width()  - m_pixbuf->get_width()) /2) - 10.0;
	const double py = y - ((allocation.get_height() - m_pixbuf->get_height())/2) - 10.0;
	bool res;

	if (px >= 0.0 && px < 25 && py >= 0.0 && py < 25)
		res = true;
	else
		res = false;

	return res;
}


void PushButton::manage_hoovered(double x, double y)
{
	bool hoovered = get_pointer_is_on_button(x, y);

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


bool PushButton::on_button_press_event(const GdkEventButton* button_event)
{
	//bool state = (button_event->type == GDK_BUTTON_PRESS && button_event->button == 1 && get_pointer_is_on_button(button_event->x, button_event->y));
	bool state = (button_event->button == 1 && get_pointer_is_on_button(button_event->x, button_event->y));

	if (state != m_state) {
		m_state = state;
		//if (m_state)
		//	std::cout << "Press" << std::endl;
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
		if (m_state)
			exec_callback();
		statusbar_update();
	}

	return true;
}


bool PushButton::on_button_release_event(const GdkEventButton* button_event)
{
	(void)button_event;

	if (m_state) {
		//std::cout << "Release" << std::endl;
		m_state = false;
		if (m_hoovered)
			m_pixbuf = m_pixbuf_off_s;
		else
			m_pixbuf = m_pixbuf_off;
		queue_draw();
		exec_callback();
		statusbar_update();
	}

	return true;
}


bool PushButton::on_scroll_event(const GdkEventScroll *scroll_event)
{
	bool state = (get_pointer_is_on_button(scroll_event->x, scroll_event->y));

	if (state && !m_state) {
		m_state = state;
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
		exec_callback();
		statusbar_update();
		Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &PushButton::on_timeout_event), 34);
	}

	return true;
}


void PushButton::on_timeout_event()
{
	if (m_state) {
		m_state = false;
		if (m_hoovered)
			m_pixbuf = m_pixbuf_off_s;
		else
			m_pixbuf = m_pixbuf_off;
		queue_draw();
		exec_callback();
		statusbar_update();
	}
}


bool PushButton::on_enter_notify_event(const GdkEventCrossing *crossing_event)
{
	manage_hoovered(crossing_event->x, crossing_event->y);
	return true;
}


bool PushButton::on_leave_notify_event(const GdkEventCrossing *crossing_event)
{
	manage_hoovered(crossing_event->x, crossing_event->y);
	//std::cout << "LEAVE" << std::endl;
	return true;
}


bool PushButton::on_motion_notify_event(const GdkEventMotion *motion_event)
{
	manage_hoovered(motion_event->x, motion_event->y);
	return true;
}


bool PushButton::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();

	if (!m_pixbuf)
		return false;

	Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf_pads, 
			(allocation.get_width() - m_pixbuf_pads->get_width())/2,
			(allocation.get_height() - m_pixbuf_pads->get_height())/2);

	cr->paint();

	Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf, 
			(allocation.get_width() - m_pixbuf->get_width())/2,
			(allocation.get_height() - m_pixbuf->get_height())/2);

	cr->paint();

	return true;
}


void PushButton::statusbar_update()
{
	if (!m_statusbar)
		return;

	m_statusbar->pop(PushButton::statusbar_context_id);
	if (m_hoovered)
		m_statusbar->push(m_button_name + std::string(" push button ") + ((m_state ^ m_active_low) ? std::string("ON") : std::string("OFF")), PushButton::statusbar_context_id);
}


