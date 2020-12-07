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

#ifndef VIRTUAL_BOARD_WINDOW_HH
#define VIRTUAL_BOARD_WINDOW_HH

#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/label.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/toolitem.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/separatortoolitem.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>
#include <glibmm/dispatcher.h>
#include "switch.hh"
#include "push_button.hh"
#include "led.hh"
#include "rgbled.hh"
#include "segseven.hh"
#include "InspectorWindow.hh"

class VirtualBoard;

class VBWindow : public Gtk::ApplicationWindow {
public:
	static const guint         toolitem_statusbar_context_id = 40;
	static const guint         simulator_statusbar_context_id = 30;
	static const guint         simulator_runstop_statusbar_context_id = 20;

protected:
	VirtualBoard              *m_virtual_board;
	Glib::Dispatcher           m_dispatcher;
	bool                       m_state_running;
	bool                       m_closing;
	unsigned int               m_ledsval;
	unsigned int               m_switchesval;
	Glib::RefPtr<Gtk::AccelGroup> m_accel_group;
	Gtk::Box                   m_boxMain;
	Gtk::Toolbar               m_toolBar1;
	Gtk::EventBox              m_boardEventBox;
	Gtk::Box                   m_boxBoard;
	Gtk::Statusbar             m_statusBar;
	Gtk::Box                   m_boxSwitchAndLed;
	Gtk::Box                   m_boxLabels;
	Gtk::Box                   m_boxSwitches;
	Gtk::Box                   m_boxLeds;
	Gtk::Box                   m_boxDisplaysAndButtons;
	Gtk::Box                   m_box_names;
	Gtk::Box                   m_boxDisplaysAndLabel;
	Gtk::Box                   m_boxDisplays;
	Gtk::Box                   m_boxDisplaysLeft;
	Gtk::Box                   m_boxDisplaysRight;
	Gtk::Box                   m_boxButtons;
	Gtk::Box                   m_boxButtonsM;
	Gtk::ToolButton            m_playpauseButton;  
	Gtk::ToolButton            m_stepButton;  
	Gtk::SeparatorToolItem     m_separator_0;
	Gtk::ToolItem              m_nstep_label_toolitem;
	Gtk::Label                 m_nstep_label;
	Gtk::ToolItem              m_nstep_toolitem;
	Gtk::SpinButton            m_nstep_spinbutton;
	Gtk::SeparatorToolItem     m_separator_1;
	Gtk::ToolItem              m_freq_label_toolitem;
	Gtk::Label                 m_freq_label;
	Gtk::ToolItem              m_freq_toolitem;
	Gtk::SpinButton            m_freq_spinbutton;
	Gtk::ToolButton            m_dumpreg_button;  
	Gtk::SeparatorToolItem     m_separator_4;
	Gtk::Label                 m_board_name;
	Gtk::Label                 m_design_name;
	Gtk::Image                 m_logo_ensta;
	Gtk::Box                   m_boxlogo;
	RGBLed                     m_rgb_1;
	PushButton                 m_rstnbnt;
	RGBLed                     m_rgb_2;
	InspectorWindow            m_inspector_window;
	sigc::connection           m_display_timeout_connection;
	int                        m_display_timeouts[8];
	unsigned char              m_display_anodes;
	unsigned char              m_display_cathodes;
	Gtk::Label                *m_labels[16];
	Switch                    *m_switches[16];
	Led                       *m_leds[16];
	SegSeven                  *m_displays[8];
	PushButton                *m_pushbuttons[5];


public:
	//VBWindow(const Glib::RefPtr<Gtk::Application>& application);
	VBWindow(VirtualBoard *virtual_board);
	virtual ~VBWindow();

	void set_freq(int v);

	void notify_from_vpi();
	void set_switch(int switchnum, bool value);
	void set_button_c(bool value);
	void set_button_u(bool value);
	void set_button_d(bool value);
	void set_button_r(bool value);
	void set_button_l(bool value);
	void set_rstn(bool value);

protected:
	/* Gtk callbacks */
	void on_run_button_clicked();
	void on_step_button_clicked();
	void on_freq_value_changed();
	void on_dump_button_clicked();
	bool on_my_delete_event(GdkEventAny* any_event);
	bool on_main_board_button_press_event(const GdkEventButton* button_event);
	bool on_my_keyboard_event(GdkEventKey* event);
	bool on_my_display_timeout();

	void on_notification_from_vpi();
};


#endif /* VIRTUAL_BOARD_WINDOW_HH */
