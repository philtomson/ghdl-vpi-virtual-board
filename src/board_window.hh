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

#ifndef ARGEN_SIMULATOR_WINDOW_HH
#define ARGEN_SIMULATOR_WINDOW_HH

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/label.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/toolitem.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/separatortoolitem.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/filefilter.h>
#include <gtkmm/image.h>
#include "switch.hh"
#include "push_button.hh"
#include "led.hh"
#include "rgbled.hh"
#include "segseven.hh"


class VBSApplication;


class VBSWindow : public Gtk::ApplicationWindow
{
public:
	static const guint         toolitem_statusbar_context_id = 40;
	static const guint         simulator_statusbar_context_id = 30;
	static const guint         simulator_runstop_statusbar_context_id = 20;

protected:
	VBSApplication *m_application;
	bool                       m_state_running;
	Gtk::Box                   m_boxMain;
	Gtk::Toolbar               m_toolBar1;
	Gtk::Toolbar               m_toolBar2;
	Gtk::Box                   m_boxBoard;
	Gtk::Statusbar             m_statusBar;
	Gtk::Box                   m_boxSwitchAndLed;
	Gtk::Box                   m_boxLabels;
	Gtk::Box                   m_boxSwitches;
	Gtk::Box                   m_boxLeds;
	Gtk::Box                   m_boxDisplaysAndButtons;
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
	Gtk::Label                 m_nvtpr_label;
	Gtk::ToolItem              m_freq_label_toolitem;
	Gtk::Label                 m_freq_label;
	Gtk::ToolItem              m_freq_toolitem;
	Gtk::SpinButton            m_freq_spinbutton;
	Gtk::ToolButton            m_dumpreg_button;  
	Gtk::SeparatorToolItem     m_separator_4;
	Gtk::ToolButton            m_resset_button;  
	Gtk::Label                 m_design_name;
	Gtk::Image                 m_logo_ensta;
	Gtk::Box                   m_boxlogo;
	RGBLed                     m_rgb_1;
	PushButton                 m_rstnbnt;
	RGBLed                     m_rgb_2;
	Gtk::Label                *m_labels[16];
	Switch                    *m_switches[16];
	Led                       *m_leds[16];
	SegSeven                  *m_displays[8];
	PushButton                *m_pushbuttons[5];


public:
	VBSWindow(VBSApplication *application);
	virtual ~VBSWindow();

	VBSApplication* get_application() { return m_application; }
	void set_freq(int v);
	void set_LEDs(unsigned short leds);
	void set_RGB_LEDs(int value, unsigned short lednum);
	void set_display(unsigned char dispnum, unsigned char segments);

	//void open_file_view(const Glib::RefPtr<Gio::File>& file);

	/* Simulator callbacks */
	void on_simulator_stopped();
	void on_simulator_running();

protected:
	/* Gtk callbacks */
	void on_run_button_clicked();
	void on_step_button_clicked();
	void on_freq_value_changed();
	void on_reset_button_clicked();
	void on_dump_button_clicked();

	void update_sensitivities();

};


#endif /* ARGEN_SIMULATOR_WINDOW_HH */
