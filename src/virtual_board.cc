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

#include <cmath>
#include <errno.h>
#include <signal.h>
#include "virtual_board.hh"

VirtualBoard::VirtualBoard() :
	m_thread(nullptr),
	m_window(nullptr),
	m_time_resolution(1.0),
	m_to_vpi_mutex(),
	m_to_vpi_condvar(),
	m_to_vpi_queue(),
	m_to_gui_mutex(),
	m_to_gui_queue(),
	m_period(0.001),
	m_timer(0),
	m_timer_is_running(false),
	cycles_to_go(0),
	design_top_unit_name(),
	simulator_name(),
	simulator_version(),
	clk_net(nullptr),
	rstn_net(nullptr),
	switches_net(nullptr),
	button_c_net(nullptr),
	button_u_net(nullptr),
	button_d_net(nullptr),
	button_r_net(nullptr),
	button_l_net(nullptr),
	leds_net(nullptr),
	display0_net(nullptr),
	display1_net(nullptr),
	display2_net(nullptr),
	display3_net(nullptr),
	display4_net(nullptr),
	display5_net(nullptr),
	display6_net(nullptr),
	display7_net(nullptr),
	rgb0_net(nullptr),
	rgb1_net(nullptr)
{
	create_timer();
}


VirtualBoard::~VirtualBoard()
{
	if (m_timer)
		timer_delete(m_timer);
	stop_gui_thread();
}


void VirtualBoard::start_gui_thread()
{
	if (m_thread)
		return;

	m_thread = new std::thread(&VirtualBoard::gui_thread_func, this);
}


int VirtualBoard::gui_thread_func()
{
	int ret;

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("fr.ensta_bretagne.bollengier.vpi_virtual_board", Gio::APPLICATION_NON_UNIQUE);

	m_window = new VBWindow(this);

	ret = app->run(*m_window);

	VBWindow *tmp = m_window;
	m_window = nullptr;
	delete tmp;

	return ret;
}


void VirtualBoard::stop_gui_thread()
{
	if (!m_thread)
		return;
	if (m_thread->joinable()) {
		printf("Joining...\n");
		m_thread->join();
		printf("Joined\n");
	}
	delete m_thread;
	m_thread = nullptr;
}


void VirtualBoard::set_time_resolution(int res)
{
	m_time_resolution = std::pow(10, -res);
}


s_vpi_time VirtualBoard::get_time(double t)
{
	s_vpi_time ts;
	uint64_t simtime = (uint64_t)(t * m_time_resolution);

	ts.type = vpiSimTime;
	ts.low = (uint32_t)(simtime & 0xffffffffUL);
	ts.high = (uint32_t)(simtime >> 32);

	return ts;
}


void VirtualBoard::send_message_to_vpi(const VBMessage& msg)
{
	std::unique_lock<std::mutex> lock(m_to_vpi_mutex);
	m_to_vpi_queue.push(msg);
	m_to_vpi_condvar.notify_one();
}


VBMessage VirtualBoard::receive_message_to_vpi()
{
	VBMessage msg;
	std::unique_lock<std::mutex> lock(m_to_vpi_mutex);

	while (m_to_vpi_queue.empty())
		m_to_vpi_condvar.wait(lock);

	msg = m_to_vpi_queue.front();
	m_to_vpi_queue.pop();

	return msg;
}


void VirtualBoard::send_message_to_gui(const VBMessage& msg)
{
	if (!m_window)
		return;
	std::unique_lock<std::mutex> lock(m_to_gui_mutex);
	m_to_gui_queue.push(msg);
	m_window->notify_from_vpi();
}


VBMessage VirtualBoard::receive_message_to_gui()
{
	VBMessage msg;
	std::unique_lock<std::mutex> lock(m_to_gui_mutex);

	if (m_to_gui_queue.empty())
		return msg;

	msg = m_to_gui_queue.front();
	m_to_gui_queue.pop();

	return msg;
}


double VirtualBoard::half_period()
{
	return m_period * 0.5;
}


static void timer_notification_callback(union sigval val)
{
	((VirtualBoard*)val.sival_ptr)->send_message_to_vpi(VBMessage::clock());
}


void VirtualBoard::create_timer()
{
	struct sigevent sevp;
	memset(&sevp, 0, sizeof(struct sigevent));
	sevp.sigev_notify = SIGEV_THREAD;
	sevp.sigev_notify_attributes = NULL;
	sevp.sigev_value.sival_ptr = this;
	sevp.sigev_notify_function = timer_notification_callback;

	if (timer_create(CLOCK_MONOTONIC, &sevp, &m_timer))
		throw std::runtime_error(std::string("timer_create(): ") + std::string(strerror(errno)));
}


void VirtualBoard::set_timer_frequency(int freq)
{
	if (freq < 1)
		freq = 1;
	else if (freq > 1000)
		freq = 1000;
	m_period = 1.0 / (double)freq;
	if (m_timer_is_running)
		start_timer(); // to set the new frequency
}


void VirtualBoard::start_timer()
{
	const long int period_nano = (long int)(1000000000.0 * m_period);
	const time_t sec = period_nano / 1000000000L;
	const long  nsec = period_nano % 1000000000L;
	const struct itimerspec new_value = {
		.it_interval = {.tv_sec = sec, .tv_nsec = nsec},
		.it_value = {.tv_sec = sec, .tv_nsec = nsec}
	};
	if (timer_settime(m_timer, 0, &new_value, NULL))
		throw std::runtime_error(std::string("timer_settime(): ") + std::string(strerror(errno)));
	m_timer_is_running = true;
}


void VirtualBoard::stop_timer()
{
	const struct itimerspec new_value = {
		.it_interval = {.tv_sec = 0, .tv_nsec = 0},
		.it_value = {.tv_sec = 0, .tv_nsec = 0}
	};
	if (timer_settime(m_timer, 0, &new_value, NULL))
		throw std::runtime_error(std::string("timer_settime(): ") + std::string(strerror(errno)));
	m_timer_is_running = false;
}


bool VirtualBoard::is_running()
{
	return m_timer_is_running;
}


