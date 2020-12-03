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

#include <gtkmm/messagedialog.h>
#include <gdkmm/rgba.h>
#include <gdkmm/pixbuf.h>
#include <iostream>
#include "board_window.hh"
#include "virtual_board.hh"

static void switch_0_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(0,  state); }
static void switch_1_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(1,  state); }
static void switch_2_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(2,  state); }
static void switch_3_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(3,  state); }
static void switch_4_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(4,  state); }
static void switch_5_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(5,  state); }
static void switch_6_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(6,  state); }
static void switch_7_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(7,  state); }
static void switch_8_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(8,  state); }
static void switch_9_callbackfun(const Switch *sw, bool state, void *arg)  { (void)sw; ((VBWindow*)arg)->set_switch(9,  state); }
static void switch_10_callbackfun(const Switch *sw, bool state, void *arg) { (void)sw; ((VBWindow*)arg)->set_switch(10, state); }
static void switch_11_callbackfun(const Switch *sw, bool state, void *arg) { (void)sw; ((VBWindow*)arg)->set_switch(11, state); }
static void switch_12_callbackfun(const Switch *sw, bool state, void *arg) { (void)sw; ((VBWindow*)arg)->set_switch(12, state); }
static void switch_13_callbackfun(const Switch *sw, bool state, void *arg) { (void)sw; ((VBWindow*)arg)->set_switch(13, state); }
static void switch_14_callbackfun(const Switch *sw, bool state, void *arg) { (void)sw; ((VBWindow*)arg)->set_switch(14, state); }
static void switch_15_callbackfun(const Switch *sw, bool state, void *arg) { (void)sw; ((VBWindow*)arg)->set_switch(15, state); }

static void push_button_center_callbackfun(const PushButton *pb, bool state, void *arg) { (void)pb; ((VBWindow*)arg)->set_button_c(state); }
static void push_button_right_callbackfun(const PushButton *pb, bool state, void *arg)  { (void)pb; ((VBWindow*)arg)->set_button_r(state); }
static void push_button_top_callbackfun(const PushButton *pb, bool state, void *arg)    { (void)pb; ((VBWindow*)arg)->set_button_u(state); }
static void push_button_left_callbackfun(const PushButton *pb, bool state, void *arg)   { (void)pb; ((VBWindow*)arg)->set_button_l(state); }
static void push_button_bottom_callbackfun(const PushButton *pb, bool state, void *arg) { (void)pb; ((VBWindow*)arg)->set_button_d(state); }
static void push_button_rstn_callbackfun(const PushButton *pb, bool state, void *arg)   { (void)pb; ((VBWindow*)arg)->set_rstn(state); }


//VBWindow::VBWindow(const Glib::RefPtr<Gtk::Application>& application) :
VBWindow::VBWindow(VirtualBoard *virtual_board) :
	Gtk::ApplicationWindow(),
	m_virtual_board(virtual_board),
	m_dispatcher(),
	m_state_running(false),
	m_closing(false),
	m_ledsval(0),
	m_switchesval(0),
	m_boxMain(Gtk::Orientation::ORIENTATION_VERTICAL),
	m_toolBar1(),
	m_boxBoard(Gtk::Orientation::ORIENTATION_VERTICAL),
	m_statusBar(),
	m_boxSwitchAndLed(Gtk::Orientation::ORIENTATION_VERTICAL),
	m_boxLabels(),
	m_boxSwitches(),
	m_boxLeds(),
	m_boxDisplaysAndButtons(),
	m_box_names(),
	m_boxDisplaysAndLabel(Gtk::Orientation::ORIENTATION_VERTICAL),
	m_boxDisplays(),
	m_boxDisplaysLeft(),
	m_boxDisplaysRight(),
	m_boxButtons(Gtk::Orientation::ORIENTATION_VERTICAL),
	m_boxButtonsM(),
	m_playpauseButton(),
	m_stepButton(),
	m_separator_0(),
	m_nstep_label_toolitem(),
	m_nstep_label("Steps:"),
	m_nstep_toolitem(),
	m_nstep_spinbutton(),
	m_separator_1(),
	m_nvtpr_label("nVTPR:"),
	m_freq_label_toolitem(),
	m_freq_label("Freq:"),
	m_freq_toolitem(),
	m_freq_spinbutton(),
	m_dumpreg_button(),
	m_separator_4(),
	m_board_name(),
	m_design_name(),
	m_logo_ensta(Gdk::Pixbuf::create_from_resource("/images/logo_ENSTA.png")),
	m_boxlogo(),
	m_rgb_1(),
	m_rstnbnt(PushButton::PUSH_BUTTON_PADS_HORIZONTAL, false),
	m_rgb_2()
{
	int i;

	set_title(std::string("Virtual Board [") + m_virtual_board->design_top_unit_name + std::string("]"));

	//set_icon_name("cpu-symbolic");
	set_icon_name("cpu-frequency-indicator");
	set_size_request(520, 312);
	m_statusBar.push("Stopped", VBWindow::simulator_runstop_statusbar_context_id);

	add(m_boxMain);

	m_toolBar1.set_toolbar_style(Gtk::TOOLBAR_ICONS); // Gtk::TOOLBAR_ICONS, Gtk::TOOLBAR_BOTH, Gtk::TOOLBAR_TEXT
	m_toolBar1.set_icon_size(Gtk::ICON_SIZE_SMALL_TOOLBAR); // ICON_SIZE_MENU: Size appropriate for menus (16px). / ICON_SIZE_SMALL_TOOLBAR: Size appropriate for small toolbars (16px). / ICON_SIZE_LARGE_TOOLBAR: Size appropriate for large toolbars (24px). / ICON_SIZE_BUTTON: Size appropriate for buttons (16px). / ICON_SIZE_DND: Size appropriate for drag and drop (32px). / ICON_SIZE_DIALOG: Size appropriate for dialogs (48px).

	m_playpauseButton.set_label("Run");
	m_playpauseButton.set_icon_name("gtk-media-play");
	m_playpauseButton.set_tooltip_text("Run simulation");
	m_playpauseButton.signal_clicked().connect(sigc::mem_fun(*this, &VBWindow::on_run_button_clicked));
	m_toolBar1.insert(m_playpauseButton, -1);

	m_stepButton.set_label("Run N cycle");
	m_stepButton.set_icon_name("gtk-media-next");
	//m_stepButton.set_icon_name("gtk-media-forward");
	m_stepButton.set_tooltip_text("Run simulation for N clock cycles");
	m_stepButton.signal_clicked().connect(sigc::mem_fun(*this, &VBWindow::on_step_button_clicked));
	m_toolBar1.insert(m_stepButton, -1);

	m_separator_0.set_draw();
	m_toolBar1.insert(m_separator_0, -1);

	m_nstep_label.set_sensitive(false);
	m_nstep_label_toolitem.add(m_nstep_label);
	m_toolBar1.insert(m_nstep_label_toolitem, -1);

	m_nstep_toolitem.set_margin_left(5);
	m_nstep_spinbutton.set_range(1, 999);
	m_nstep_spinbutton.set_digits(0);
	m_nstep_spinbutton.set_numeric();
	m_nstep_spinbutton.set_increments(1, 10);
	m_nstep_spinbutton.set_tooltip_text("Number of clock cycles to run");
	m_nstep_toolitem.add(m_nstep_spinbutton);
	m_toolBar1.insert(m_nstep_toolitem, -1);

	m_separator_1.set_draw();
	//m_separator_1.set_expand();
	m_toolBar1.insert(m_separator_1, -1);

	m_freq_label_toolitem.add(m_freq_label);
	m_freq_label.set_sensitive(false);
	m_toolBar1.insert(m_freq_label_toolitem, -1);

	m_freq_toolitem.set_margin_left(5);
	m_freq_spinbutton.set_range(1, 1000);
	m_freq_spinbutton.set_value(1000);
	m_freq_spinbutton.set_digits(0);
	m_freq_spinbutton.set_numeric();
	m_freq_spinbutton.set_increments(1, 10);
	m_freq_spinbutton.set_tooltip_text("Clock cycles per second (to limit simulator speed)");
	m_freq_spinbutton.signal_value_changed().connect(sigc::mem_fun(*this, &VBWindow::on_freq_value_changed));
	m_freq_toolitem.add(m_freq_spinbutton);
	m_toolBar1.insert(m_freq_toolitem, -1);

	m_separator_4.set_draw();
	//m_separator_4.set_expand();
	m_toolBar1.insert(m_separator_4, -1);

	m_dumpreg_button.set_label("Inspect signals");
	m_dumpreg_button.set_icon_name("gtk-find");
	m_dumpreg_button.set_tooltip_text("Inspect internal signals");
	m_dumpreg_button.signal_clicked().connect(sigc::mem_fun(*this, &VBWindow::on_dump_button_clicked));
	m_toolBar1.insert(m_dumpreg_button, -1);

	m_boxMain.pack_start(m_toolBar1, Gtk::PACK_SHRINK, 0);

	m_boxMain.pack_start(m_boxBoard, Gtk::PACK_EXPAND_WIDGET, 0);
	m_boxMain.pack_start(m_statusBar, Gtk::PACK_SHRINK, 0);

	m_boxBoard.override_background_color(Gdk::RGBA("rgb(43, 187, 77)"), Gtk::StateFlags::STATE_FLAG_NORMAL);

	m_pushbuttons[0] = new PushButton(PushButton::PUSH_BUTTON_PADS_HORIZONTAL); m_pushbuttons[0]->set_statusbar(m_statusBar, "Center");
	m_pushbuttons[1] = new PushButton(PushButton::PUSH_BUTTON_PADS_VERTICAL);   m_pushbuttons[1]->set_statusbar(m_statusBar, "Right");
	m_pushbuttons[2] = new PushButton(PushButton::PUSH_BUTTON_PADS_HORIZONTAL); m_pushbuttons[2]->set_statusbar(m_statusBar, "Top");
	m_pushbuttons[3] = new PushButton(PushButton::PUSH_BUTTON_PADS_VERTICAL);   m_pushbuttons[3]->set_statusbar(m_statusBar, "Left");
	m_pushbuttons[4] = new PushButton(PushButton::PUSH_BUTTON_PADS_HORIZONTAL); m_pushbuttons[4]->set_statusbar(m_statusBar, "Bottom");
	m_pushbuttons[0]->set_callback_function(push_button_center_callbackfun, this);
	m_pushbuttons[1]->set_callback_function(push_button_right_callbackfun,  this);
	m_pushbuttons[2]->set_callback_function(push_button_top_callbackfun,    this);
	m_pushbuttons[3]->set_callback_function(push_button_left_callbackfun,   this);
	m_pushbuttons[4]->set_callback_function(push_button_bottom_callbackfun, this);
	m_rstnbnt.set_callback_function(push_button_rstn_callbackfun, this);
	m_rstnbnt.set_statusbar(m_statusBar, "RSTN");

	m_boxButtons.pack_start(*m_pushbuttons[2], Gtk::PACK_SHRINK);
	m_boxButtonsM.pack_start(*m_pushbuttons[3], Gtk::PACK_SHRINK);
	m_boxButtonsM.pack_start(*m_pushbuttons[0], Gtk::PACK_SHRINK);
	m_boxButtonsM.pack_start(*m_pushbuttons[1], Gtk::PACK_SHRINK);
	m_boxButtons.pack_start(m_boxButtonsM, Gtk::PACK_SHRINK);
	m_boxButtons.pack_start(*m_pushbuttons[4], Gtk::PACK_SHRINK);

	m_displays[0] = new SegSeven(SegSeven::SEGSEVEN_RIGHT);
	m_displays[1] = new SegSeven(SegSeven::SEGSEVEN_MIDDLE);
	m_displays[2] = new SegSeven(SegSeven::SEGSEVEN_MIDDLE);
	m_displays[3] = new SegSeven(SegSeven::SEGSEVEN_LEFT);
	m_displays[4] = new SegSeven(SegSeven::SEGSEVEN_RIGHT);
	m_displays[5] = new SegSeven(SegSeven::SEGSEVEN_MIDDLE);
	m_displays[6] = new SegSeven(SegSeven::SEGSEVEN_MIDDLE);
	m_displays[7] = new SegSeven(SegSeven::SEGSEVEN_LEFT);

	m_boxDisplaysRight.pack_start(*m_displays[3], Gtk::PACK_SHRINK);
	m_boxDisplaysRight.pack_start(*m_displays[2], Gtk::PACK_SHRINK);
	m_boxDisplaysRight.pack_start(*m_displays[1], Gtk::PACK_SHRINK);
	m_boxDisplaysRight.pack_start(*m_displays[0], Gtk::PACK_SHRINK);

	m_boxDisplaysLeft.pack_start(*m_displays[7], Gtk::PACK_SHRINK);
	m_boxDisplaysLeft.pack_start(*m_displays[6], Gtk::PACK_SHRINK);
	m_boxDisplaysLeft.pack_start(*m_displays[5], Gtk::PACK_SHRINK);
	m_boxDisplaysLeft.pack_start(*m_displays[4], Gtk::PACK_SHRINK);

	m_boxDisplaysRight.set_margin_left(20);

	std::string simulator_version = std::string(m_virtual_board->simulator_version);
	simulator_version = simulator_version.substr(0, simulator_version.find(" ("));
	m_board_name.set_label(
			std::string("VPI Virtual Board on ") +
			std::string(m_virtual_board->simulator_name) +
			std::string(" ") +
			std::string(simulator_version));
	m_board_name.override_color(Gdk::RGBA("white"));
	//m_board_name.set_justify(Gtk::JUSTIFY_CENTER);
	m_board_name.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
	m_box_names.pack_start(m_board_name, Gtk::PACK_EXPAND_WIDGET, 0);

	m_design_name.set_label(std::string("[") + m_virtual_board->design_top_unit_name + std::string("]"));
	m_design_name.override_color(Gdk::RGBA("white"));
	//m_design_name.set_justify(Gtk::JUSTIFY_RIGHT);
	m_design_name.set_alignment(Gtk::ALIGN_END, Gtk::ALIGN_CENTER);
	m_box_names.pack_end(m_design_name, Gtk::PACK_EXPAND_WIDGET, 0);

	m_boxDisplaysAndLabel.pack_start(m_box_names, Gtk::PACK_SHRINK, 7);

	m_boxDisplays.pack_start(m_boxDisplaysLeft,  Gtk::PACK_SHRINK, 0);
	m_boxDisplays.pack_start(m_boxDisplaysRight, Gtk::PACK_SHRINK, 0);
	m_boxDisplaysAndLabel.pack_start(m_boxDisplays, Gtk::PACK_EXPAND_PADDING, 0);
	//m_boxDisplaysAndLabel.pack_start(m_boxDisplays, Gtk::PACK_EXPAND_PADDING, 0);

	//m_boxDisplaysAndLabel.pack_start(m_logo_ensta, Gtk::PACK_SHRINK, 0);
	m_logo_ensta.set_margin_top(5);
	m_boxlogo.pack_start(m_logo_ensta, Gtk::PACK_EXPAND_WIDGET, 0);
	m_boxDisplaysAndLabel.pack_start(m_boxlogo, Gtk::PACK_EXPAND_WIDGET, 0);
	m_logo_ensta.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
	m_boxlogo.pack_start(m_rgb_1, Gtk::PACK_EXPAND_PADDING, 0);
	m_boxlogo.pack_start(m_rstnbnt, Gtk::PACK_EXPAND_PADDING, 0);
	m_boxlogo.pack_start(m_rgb_2, Gtk::PACK_EXPAND_PADDING, 0);


	m_boxDisplaysAndButtons.pack_start(m_boxDisplaysAndLabel, Gtk::PACK_EXPAND_PADDING, 5);
	m_boxDisplaysAndButtons.pack_start(m_boxButtons, Gtk::PACK_EXPAND_PADDING, 5);


	m_boxBoard.pack_start(m_boxDisplaysAndButtons, Gtk::PACK_EXPAND_PADDING, 0);

	for (i = 0; i < 16; i++) {
		m_leds[i] = new Led();
		m_switches[i] = new Switch();
		m_labels[i] = new Gtk::Label(std::to_string(i));
		m_labels[i]->override_color(Gdk::RGBA("white"));
		m_labels[i]->set_justify(Gtk::JUSTIFY_CENTER);
		m_boxLeds.pack_end(*m_leds[i], Gtk::PACK_EXPAND_PADDING, 0);
		m_boxSwitches.pack_end(*m_switches[i], Gtk::PACK_EXPAND_PADDING, 0);
		m_boxLabels.pack_end(*m_labels[i], Gtk::PACK_EXPAND_PADDING, 0);
		m_switches[i]->set_statusbar(m_statusBar, i);
	}
	m_switches[0]->set_callback_function(switch_0_callbackfun, this);
	m_switches[1]->set_callback_function(switch_1_callbackfun, this);
	m_switches[2]->set_callback_function(switch_2_callbackfun, this);
	m_switches[3]->set_callback_function(switch_3_callbackfun, this);
	m_switches[4]->set_callback_function(switch_4_callbackfun, this);
	m_switches[5]->set_callback_function(switch_5_callbackfun, this);
	m_switches[6]->set_callback_function(switch_6_callbackfun, this);
	m_switches[7]->set_callback_function(switch_7_callbackfun, this);
	m_switches[8]->set_callback_function(switch_8_callbackfun, this);
	m_switches[9]->set_callback_function(switch_9_callbackfun, this);
	m_switches[10]->set_callback_function(switch_10_callbackfun, this);
	m_switches[11]->set_callback_function(switch_11_callbackfun, this);
	m_switches[12]->set_callback_function(switch_12_callbackfun, this);
	m_switches[13]->set_callback_function(switch_13_callbackfun, this);
	m_switches[14]->set_callback_function(switch_14_callbackfun, this);
	m_switches[15]->set_callback_function(switch_15_callbackfun, this);

	m_boxLeds.set_homogeneous();
	m_boxSwitches.set_homogeneous();
	m_boxLabels.set_homogeneous();
	m_boxSwitchAndLed.pack_start(m_boxLeds, Gtk::PACK_SHRINK, 0);
	m_boxSwitchAndLed.pack_start(m_boxSwitches, Gtk::PACK_SHRINK, 0);
	m_boxSwitchAndLed.pack_start(m_boxLabels, Gtk::PACK_SHRINK, 5);
	m_boxSwitchAndLed.set_margin_left(5);
	m_boxSwitchAndLed.set_margin_right(5);

	m_boxBoard.pack_end(m_boxSwitchAndLed, Gtk::PACK_SHRINK, 0);

	m_dispatcher.connect(sigc::mem_fun(*this, &VBWindow::on_notification_from_vpi));

	show_all();

	m_virtual_board->send_message_to_vpi(VBMessage::gui_started());

//	m_boxMain.pack_start(led1, Gtk::PACK_SHRINK, 0);
//	m_boxMain.pack_start(led1, Gtk::PACK_EXPAND_PADDING, 0);
}


VBWindow::~VBWindow()
{
	int i;

	for (i = 0; i < 16; i++) {
		delete m_leds[i];
		delete m_switches[i];
		delete m_labels[i];
	}

	for (i = 0; i < 8; i++)
		delete m_displays[i];

	for (i = 0; i < 5; i++)
		delete m_pushbuttons[i];
}


void VBWindow::on_run_button_clicked()
{
	if (m_state_running)
		m_virtual_board->send_message_to_vpi(VBMessage::stop());
	else
		m_virtual_board->send_message_to_vpi(VBMessage::run());
}


void VBWindow::on_step_button_clicked()
{
	m_virtual_board->send_message_to_vpi(VBMessage::run_n((unsigned int)m_nstep_spinbutton.get_value()));
}


void VBWindow::on_freq_value_changed()
{
	m_virtual_board->send_message_to_vpi(VBMessage::set_freq((unsigned int)m_freq_spinbutton.get_value()));
}


void VBWindow::on_dump_button_clicked()
{
	m_virtual_board->send_message_to_vpi(VBMessage::update_signals());
}


bool VBWindow::on_delete_event(GdkEventAny* any_event)
{
	(void)any_event;
	m_closing = true;
	m_virtual_board->send_message_to_vpi(VBMessage::exit());
	return false;
}


void VBWindow::notify_from_vpi()
{
	m_dispatcher.emit();
}


void VBWindow::set_switch(int switchnum, bool value)
{
	if (switchnum < 0 || switchnum >= 16)
		return;

	if (value)
		m_switchesval |= (1 << switchnum);
	else
		m_switchesval &= ~(1 << switchnum);
	m_virtual_board->send_message_to_vpi(VBMessage::io_changed(VBMessage::SWITCHES, m_switchesval));
}


void VBWindow::set_button_c(bool value)
{
	m_virtual_board->send_message_to_vpi(VBMessage::io_changed(VBMessage::PUSH_BUTTON_CENTER, (int)value));
}


void VBWindow::set_button_u(bool value)
{
	m_virtual_board->send_message_to_vpi(VBMessage::io_changed(VBMessage::PUSH_BUTTON_UP, (int)value));
}


void VBWindow::set_button_d(bool value)
{
	m_virtual_board->send_message_to_vpi(VBMessage::io_changed(VBMessage::PUSH_BUTTON_DOWN, (int)value));
}


void VBWindow::set_button_r(bool value)
{
	m_virtual_board->send_message_to_vpi(VBMessage::io_changed(VBMessage::PUSH_BUTTON_RIGHT, (int)value));
}


void VBWindow::set_button_l(bool value)
{
	m_virtual_board->send_message_to_vpi(VBMessage::io_changed(VBMessage::PUSH_BUTTON_LEFT, (int)value));
}


void VBWindow::set_rstn(bool value)
{
	m_virtual_board->send_message_to_vpi(VBMessage::io_changed(VBMessage::PUSH_BUTTON_RSTN, (int)value));
}


void VBWindow::on_notification_from_vpi()
{
	VBMessage msg;
	bool try_to_read_other_messages = true;
	unsigned int val;
	unsigned int i, b;

	//printf("on_notification_from_vpi():\n");
	msg = m_virtual_board->receive_message_to_gui();
	while (try_to_read_other_messages) {
		switch (msg.type()) {
			case VBMessage::MSG_NONE:
				//printf("MSG_NONE\n");
				try_to_read_other_messages = false;
				break;
			case VBMessage::MSG_EXIT:
				try_to_read_other_messages = false;
				if (!m_closing)
					get_application()->quit();
				break;
			case VBMessage::MSG_IO_CHANGED:
				//printf("\e[32mMSG_IO_CHANGED\e[0m\n");
				val = msg.value();
				switch (msg.io_name()) {
					case VBMessage::LEDS:
						for (i = 0, b = 1; i < 16; i++, b <<= 1) {
							if ((val & b) ^ (m_ledsval & b))
								m_leds[i]->set_state((bool)((val & b) != 0));
						}
						m_ledsval = val;
						break;
					case VBMessage::RGB_LED_0:
						m_rgb_1.set_state(val);
						break;
					case VBMessage::RGB_LED_1:
						m_rgb_2.set_state(val);
						break;
					case VBMessage::DISPLAY_0:
						m_displays[0]->set_state(val);
						break;
					case VBMessage::DISPLAY_1:
						m_displays[1]->set_state(val);
						break;
					case VBMessage::DISPLAY_2:
						m_displays[2]->set_state(val);
						break;
					case VBMessage::DISPLAY_3:
						m_displays[3]->set_state(val);
						break;
					case VBMessage::DISPLAY_4:
						m_displays[4]->set_state(val);
						break;
					case VBMessage::DISPLAY_5:
						m_displays[5]->set_state(val);
						break;
					case VBMessage::DISPLAY_6:
						m_displays[6]->set_state(val);
						break;
					case VBMessage::DISPLAY_7:
						m_displays[7]->set_state(val);
						break;
					default:
						break;
				}
				break;
			case VBMessage::MSG_STOPPED:
				//printf("MSG_STOPPED\n");
				m_state_running = false;
				m_statusBar.pop(VBWindow::simulator_runstop_statusbar_context_id);
				m_statusBar.push("Stopped", VBWindow::simulator_runstop_statusbar_context_id);
				m_playpauseButton.set_label("Run");
				m_playpauseButton.set_icon_name("gtk-media-play");
				m_playpauseButton.set_tooltip_text("Run simulation");
				m_stepButton.set_sensitive(true);
				break;
			case VBMessage::MSG_RUNNING:
				//printf("MSG_RUNNING\n");
				m_state_running = true;
				m_statusBar.pop(VBWindow::simulator_runstop_statusbar_context_id);
				m_statusBar.push("Running", VBWindow::simulator_runstop_statusbar_context_id);
				m_playpauseButton.set_label("Stop");
				m_playpauseButton.set_icon_name("gtk-media-pause");
				m_playpauseButton.set_tooltip_text("Stop simulation");
				m_stepButton.set_sensitive(false);
				break;
			case VBMessage::MSG_SIGNALS_UPDATED:
				printf("MSG_SIGNALS_UPDATED\n");
				/* TODO */
				break;
			default:
				printf("\e[31mBad MSG: \"%s\" (%d)\e[0m\n", msg.type_to_s(), msg.type());
				try_to_read_other_messages = false;
				break;
		}
		if (try_to_read_other_messages)
			msg = m_virtual_board->receive_message_to_gui();
	}
}


