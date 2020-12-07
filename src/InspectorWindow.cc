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

#include <gtkmm/treeselection.h>
#include <cmath>
#include "InspectorWindow.hh"
#include "virtual_board.hh"
#include <iostream>


SignalTreeView::SignalTreeView() :
	Gtk::TreeView(),
	m_binary_menuitem("Binary"),
	m_hexa_menuitem("Hexa"),
	m_decimal_menuitem("Decimal"),
	m_menu(),
	m_model_columns(nullptr)
{
	m_menu.append(m_binary_menuitem);
	m_binary_menuitem.signal_activate().connect(sigc::mem_fun(*this, &SignalTreeView::on_binary_selected));
	m_menu.append(m_hexa_menuitem);
	m_hexa_menuitem.signal_activate().connect(sigc::mem_fun(*this, &SignalTreeView::on_hexa_selected));
	m_menu.append(m_decimal_menuitem);
	m_decimal_menuitem.signal_activate().connect(sigc::mem_fun(*this, &SignalTreeView::on_decimal_selected));
	get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
	m_menu.show_all();
}


SignalTreeView::~SignalTreeView()
{
}


void SignalTreeView::set_column_model(const NetModelColumns& model_columns)
{
	m_model_columns = &model_columns;
}


bool SignalTreeView::on_button_press_event(GdkEventButton *button_event)
{
	bool ret = true;

	if (get_selection()->count_selected_rows() < 2 || button_event->type != GDK_BUTTON_PRESS || button_event->button != 3) {
		ret = TreeView::on_button_press_event(button_event);
	}

	if ((button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3) && get_selection()->count_selected_rows() > 0)
		m_menu.popup_at_pointer((GdkEvent*)button_event);

	return ret;
}


void SignalTreeView::on_binary_selected()
{
	const Glib::RefPtr<Gtk::TreeSelection> selection = get_selection();
	Glib::RefPtr<Gtk::TreeModel> model = selection->get_model();
	std::vector<Gtk::TreeModel::Path> paths = selection->get_selected_rows();
	for (std::vector<Gtk::TreeModel::Path>::const_iterator path = paths.cbegin(); path != paths.cend(); ++path) {
		Gtk::TreeModel::iterator iter = model->get_iter(*path);
		if (iter)
			(*iter)[m_model_columns->m_col_format] = 0;
	}
//	Glib::RefPtr<Gtk::TreeSelection> ref_selection = get_selection();
//	if (ref_selection) {
//		Gtk::TreeModel::iterator iter = ref_selection->get_selected();
//		if (iter) {
//			(*iter)[m_model_columns->m_col_format] = 0;
//		}
//	}
}


void SignalTreeView::on_decimal_selected()
{
	const Glib::RefPtr<Gtk::TreeSelection> selection = get_selection();
	Glib::RefPtr<Gtk::TreeModel> model = selection->get_model();
	std::vector<Gtk::TreeModel::Path> paths = selection->get_selected_rows();
	for (std::vector<Gtk::TreeModel::Path>::const_iterator path = paths.cbegin(); path != paths.cend(); ++path) {
		Gtk::TreeModel::iterator iter = model->get_iter(*path);
		if (iter)
			(*iter)[m_model_columns->m_col_format] = 1;
	}
}


void SignalTreeView::on_hexa_selected()
{
	const Glib::RefPtr<Gtk::TreeSelection> selection = get_selection();
	Glib::RefPtr<Gtk::TreeModel> model = selection->get_model();
	std::vector<Gtk::TreeModel::Path> paths = selection->get_selected_rows();
	for (std::vector<Gtk::TreeModel::Path>::const_iterator path = paths.cbegin(); path != paths.cend(); ++path) {
		Gtk::TreeModel::iterator iter = model->get_iter(*path);
		if (iter)
			(*iter)[m_model_columns->m_col_format] = 2;
	}
}



InspectorWindow::InspectorWindow(VirtualBoard* vb) :
	Gtk::Window(),
	m_virtual_board(vb),
//	m_headerbar(),
	m_paned(),
	m_frame_modules(),
	m_frame_nets(),
	m_scrollwindow_modules(),
	m_scrollwindow_nets(),
	m_net_value_renderer(),
	m_net_value_treeviewcolumn(),
	m_curren_module(nullptr),
	m_timeout_connection()
{
	set_icon_name("gtk-find");
	set_title("Signal inspector");
//	m_headerbar.set_title("Signal inspector");
//	m_headerbar.set_show_close_button();
//	set_titlebar(m_headerbar);
	signal_show().connect(sigc::mem_fun(*this, &InspectorWindow::on_my_show));
	signal_hide().connect(sigc::mem_fun(*this, &InspectorWindow::on_my_hide));
	signal_delete_event().connect(sigc::mem_fun(*this, &InspectorWindow::on_my_delete_event));
	set_size_request(640, 480);
	//set_border_width(5);
//	m_paned.set_margin_bottom(5);
//	m_paned.set_margin_top(0);
	m_paned.set_position(213);
	//m_paned.set_wide_handle();
	add(m_paned);
	m_scrollwindow_modules.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scrollwindow_nets.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
#if 1
	set_border_width(8);
	//m_paned.set_wide_handle();
	m_frame_modules.add(m_scrollwindow_modules);
	m_frame_modules.set_shadow_type(Gtk::SHADOW_IN);
	m_frame_nets.add(m_scrollwindow_nets);
	m_frame_nets.set_shadow_type(Gtk::SHADOW_IN);
	m_frame_nets.set_margin_left(8);
	m_frame_modules.set_margin_right(8);
	m_paned.pack1(m_frame_modules, true, true);
	m_paned.pack2(m_frame_nets, true, false);
#else
	m_scrollwindow_modules.set_margin_bottom(5);
	m_scrollwindow_modules.set_margin_right(1);
    m_scrollwindow_nets.set_margin_bottom(5);
    m_scrollwindow_nets.set_margin_left(1);
	m_paned.pack1(m_scrollwindow_modules, true, true);
	m_paned.pack2(m_scrollwindow_nets, true, true);
#endif

	m_scrollwindow_modules.add(m_module_treeview);
	m_scrollwindow_nets.add(m_net_treeview);



	/* Module hierarchy */
	m_module_ref_tree_model = Gtk::TreeStore::create(m_module_model_column);

	Gtk::TreeModel::Row row = *(m_module_ref_tree_model->append());
	build_module_hierarchy_model(row, m_virtual_board->top_module);


	//Add the TreeView's view columns:
	m_module_treeview.append_column("Modules", m_module_model_column.m_col_name);
	m_module_treeview.set_enable_tree_lines();
	m_module_treeview.set_show_expanders();
	m_module_treeview.set_enable_search();
	m_module_treeview.get_column(0)->set_sort_column(m_net_model_column.m_col_name);

	m_module_treeview.set_model(m_module_ref_tree_model);
	m_module_treeview.expand_all();
	m_module_treeview.set_activate_on_single_click();
	m_module_treeview.signal_row_activated().connect(sigc::mem_fun(*this, &InspectorWindow::on_module_treeview_row_activated));


	/* NETs */
	m_net_treeview.set_column_model(m_net_model_column);
	/* Add the TreeView's view columns */
	m_net_treeview.append_column("Nets",  m_net_model_column.m_col_name);
	m_net_treeview.append_column("W",     m_net_model_column.m_col_width);
	m_net_treeview.append_column("IO",    m_net_model_column.m_col_type);

	m_net_value_treeviewcolumn.set_title("Value");
	m_net_value_treeviewcolumn.pack_start(m_net_value_renderer);
	m_net_treeview.append_column(m_net_value_treeviewcolumn);
	//Tell the view column how to render the model values:
	m_net_value_treeviewcolumn.set_cell_data_func(m_net_value_renderer,
			sigc::mem_fun(*this, &InspectorWindow::treeviewcolumn_net_value_on_cell_data));
	m_net_value_renderer.property_family() = "Monospace";
	//m_net_value_renderer.property_style() = Pango::STYLE_ITALIC;
//	// Make the CellRenderer editable, and handle its editing signals:
//	m_net_value_renderer.property_editable() = true;
//	m_net_value_renderer.signal_edited().connect(sigc::mem_fun(*this, &InspectorWindow::on_net_value_edited));

	m_net_treeview.get_column(0)->set_min_width(25);
	m_net_treeview.get_column(1)->set_alignment(Gtk::ALIGN_CENTER);
	m_net_treeview.get_column(2)->set_alignment(Gtk::ALIGN_CENTER);
	m_net_treeview.get_column(1)->set_min_width(25);
	m_net_treeview.get_column(1)->set_max_width(40);
	//m_net_treeview.get_column(1)->get_first_cell()->set_alignment(1.0f, 0.5f);
	m_net_treeview.get_column_cell_renderer(1)->set_alignment(1.0f, 0.5f);
	m_net_treeview.get_column(2)->set_max_width(25);
	m_net_treeview.get_column(2)->set_min_width(25);
	//m_net_treeview.get_column(2)->get_first_cell()->set_alignment(0.5f, 0.5f);
	m_net_treeview.get_column_cell_renderer(2)->set_alignment(0.5f, 0.5f);
	m_net_treeview.get_column(3)->set_min_width(30);

	m_net_treeview.set_rules_hint();
	//m_net_treeview.set_rubber_banding();
	//m_net_treeview.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH); //TREE_VIEW_GRID_LINES_HORIZONTAL
	m_net_treeview.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_VERTICAL); //TREE_VIEW_GRID_LINES_HORIZONTAL


	for (guint i = 0; i < 4; i++) {
		m_net_treeview.get_column(i)->set_reorderable();
		m_net_treeview.get_column(i)->set_resizable();
		//m_net_treeview.get_column(i)->set_sizing(Gtk::TREE_VIEW_COLUMN_AUTOSIZE);
		m_net_treeview.get_column(i)->set_expand(false);
	}

	//m_net_treeview.columns_autosize();
	//m_net_treeview.check_resize();
	
	m_net_treeview.set_reorderable();
	m_net_treeview.get_column(0)->set_sort_column(m_net_model_column.m_col_name);
	m_net_treeview.get_column(1)->set_sort_column(m_net_model_column.m_col_width);
	m_net_treeview.get_column(2)->set_sort_column(m_net_model_column.m_col_type);
	m_net_treeview.set_enable_search();
	m_net_treeview.set_activate_on_single_click();
//	m_net_treeview.signal_row_activated().connect(sigc::mem_fun(*this, &InspectorWindow::on_net_treeview_row_activated));

	Gtk::TreeModel::iterator iter = m_module_ref_tree_model->get_iter("0");
	if (iter) {
		row = *iter;
		Glib::RefPtr<Gtk::ListStore> top_module_net_list = row[m_module_model_column.m_col_net_model];
		m_net_treeview.set_model(top_module_net_list);
	}
	m_curren_module = &m_virtual_board->top_module;
}


bool InspectorWindow::on_my_delete_event(GdkEventAny* any_event)
{
	(void)any_event;
	hide();
	return true;
}


void InspectorWindow::on_module_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
	(void)column;
	Gtk::TreeModel::iterator iter = m_module_ref_tree_model->get_iter(path);
	if (iter) {
		Gtk::TreeModel::Row row = *iter;
		//std::cout << "Row activated: " << std::string(row[m_module_model_column.m_col_name]) << std::endl;
		Glib::RefPtr<Gtk::ListStore> net_list_model = row[m_module_model_column.m_col_net_model];
		m_net_treeview.set_model(net_list_model);
/* Keep ? */
		//m_virtual_board->send_message_to_vpi(VBMessage::read_module_nets((ModuleInstance*)row[m_module_model_column.m_col_module_instance]));

		m_curren_module = (ModuleInstance*)row[m_module_model_column.m_col_module_instance];
	}
}


/* Keep ? */
// void InspectorWindow::on_net_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
// {
// 	(void)column;
// 	Gtk::TreeModel::iterator iter = column->get_tree_view()->get_model()->get_iter(path);
// 	if (iter) {
// 		Gtk::TreeModel::Row row = *iter;
// 		std::cout << "Row activated: " << std::string(row[m_net_model_column.m_col_name]) << std::endl;
// 		m_virtual_board->send_message_to_vpi(VBMessage::read_net((ModuleNet*)row[m_net_model_column.m_col_net]));
// 	}
// }


void InspectorWindow::update_module_signals_model(const ModuleInstance& mi)
{
	Glib::RefPtr<Gtk::TreeModel> signal_liststore = mi.signal_liststore;
	Gtk::TreeModel::Children children = signal_liststore->children();
	for (Gtk::TreeModel::Children::iterator iter = children.begin(); iter != children.end(); ++iter) {
		Gtk::TreeModel::Row row = *iter;
		if (((ModuleNet*)row[m_net_model_column.m_col_net])->value_changed)
			row[m_net_model_column.m_col_dumy] = row[m_net_model_column.m_col_dumy] xor true;
	}
}


void InspectorWindow::build_module_hierarchy_model(Gtk::TreeModel::Row& module_row, ModuleInstance& inst)
{
	int i, s;
	Glib::RefPtr<Gtk::ListStore> net_ref_tree_model = Gtk::ListStore::create(m_net_model_column);

	for (std::vector<ModuleNet>::iterator it = inst.nets.begin(); it != inst.nets.end(); ++it) {
		Gtk::TreeModel::Row net_row = *(net_ref_tree_model->append());
		net_row[m_net_model_column.m_col_name] = it->name;
		net_row[m_net_model_column.m_col_width] = it->width;
		switch (it->direction) {
			case 1:
				net_row[m_net_model_column.m_col_type] = "i";
				break;
			case 2:
				net_row[m_net_model_column.m_col_type] = "o";
				break;
			case 3:
				net_row[m_net_model_column.m_col_type] = "io";
				break;
			default:
				net_row[m_net_model_column.m_col_type] = "";
		}
		net_row[m_net_model_column.m_col_net] = &(*it);
	}

	module_row[m_module_model_column.m_col_net_model] = net_ref_tree_model;

	module_row[m_module_model_column.m_col_name] = inst.name;
	module_row[m_module_model_column.m_col_module_instance] = &inst;

	inst.signal_liststore = net_ref_tree_model;

	s = inst.modules.size();
	for (i = 0; i < s; i++) {
		Gtk::TreeModel::Row childrow = *(m_module_ref_tree_model->append(module_row.children()));
		build_module_hierarchy_model(childrow, inst.modules[i]);
	}
}


void InspectorWindow::treeviewcolumn_net_value_on_cell_data(
		Gtk::CellRenderer *renderer,
		const Gtk::TreeModel::iterator& iter)
{
	(void)renderer;
	//Get the value from the model and show it appropriately in the view:
	if(iter) {
		Gtk::TreeModel::Row row = *iter;
		const ModuleNet *modnet = row[m_net_model_column.m_col_net];
		switch (row[m_net_model_column.m_col_format]) {
			case 1:
				m_net_value_renderer.property_text() = binary_to_decimal(modnet->value);
				break;
			case 2:
				m_net_value_renderer.property_text() = binary_to_hexa(modnet->value);
				break;
			default:
				m_net_value_renderer.property_text() = modnet->value;
		}
		//m_net_value_renderer.property_text() = std::string("Coucou les amis");
	}
}


/* FIXME convert to decimal strings greater than 64 bits */
std::string InspectorWindow::binary_to_decimal(const std::string& binary)
{
	bool valid = true;
	unsigned long val = 0;
	unsigned int totlen = (int)std::ceil(std::log10(std::pow(2, binary.size())));
	for (std::string::const_iterator it = binary.cbegin(); it != binary.cend(); ++it) {
		val <<= 1;
		if (*it == '1' || *it == 'H')
			val |= 1;
		else if (*it != '0' && *it != 'L') {
			valid = false;
			break;
		}
	}
	if (!valid)
		return std::string("Unknown");

	std::string np = std::to_string(val);

	if (np.size() >= totlen)
		return np;
	return std::string(totlen - np.size(), ' ') + np;
}


std::string InspectorWindow::binary_to_hexa(const std::string& binary)
{
	bool valid;
	int strs = (binary.size() - 1) / 4 + 1;
	std::string res(strs, '0');
	int i, j, k;
	char c, v;
	for (i = strs-1, j = binary.size() - 1; i >= 0; i--) {
		valid = true;
		v = 0;
		for (k = 0; k < 4 && j >= 0; k++, j--) {
			c = binary[j];
			if (c == '1' || c == 'H')
				v |= (1 << k);
			else if (c != '0' && c != 'L')
				valid = false;
		}
		if (v > 9)
			v += ('a' - 10);
		else
			v += '0';
		if (valid) 
			res[i] = v;
		else
			res[i] = 'X';
	}

	return res;
}


void InspectorWindow::on_my_show()
{
	//std::cout << "Show inspector" << std::endl;
	m_timeout_connection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &InspectorWindow::on_my_timeout), 33);
}

void InspectorWindow::on_my_hide()
{
	//std::cout << "Hide inspector" << std::endl;
	m_timeout_connection.disconnect();
}


bool InspectorWindow::on_my_timeout()
{
	//std::cout << "on_my_timeout()" << std::endl;
	m_virtual_board->send_message_to_vpi(VBMessage::read_module_nets(m_curren_module));
	return true;
}


// void InspectorWindow::on_net_value_edited(const Glib::ustring& path, const Glib::ustring& new_text)
// {
// 	Glib::RefPtr<Gtk::TreeModel> model = m_net_treeview.get_model();
// 	Gtk::TreeModel::iterator iter = model->get_iter(path);
// 	if (iter) {
// 		switch ((*iter)[m_net_model_column.m_col_format]) {
// 			case 0: // binary
// 				put_binary_value(new_text, (ModuleNet*)((*iter)[m_net_model_column.m_col_net]));
// 				break;
// 			case 1: // decimal
// 				put_decimal_value(new_text, (ModuleNet*)((*iter)[m_net_model_column.m_col_net]));
// 				std::cout << "Decimal" << std::endl;
// 				break;
// 			case 2: // hexa
// 				put_hexa_value(new_text, (ModuleNet*)((*iter)[m_net_model_column.m_col_net]));
// 				std::cout << "Hexa" << std::endl;
// 				break;
// 		}
// 	}
// 	std::cout << path << " -> " << new_text << std::endl;
// }
// 
// 
// void InspectorWindow::put_binary_value(const Glib::ustring& user_string, ModuleNet *net)
// {
// 	int i, j;
// 	char c;
// 	bool valid = true;
// 	net->value_to_force = std::string(net->width, '0');
// 	for (i = net->width - 1, j = user_string.size() - 1; i >= 0 && j >= 0; i--, j--) {
// 		c = user_string[j];
// 		switch (c) {
// 			case 'U':
// 			case 'X':
// 			case '0':
// 			case '1':
// 			case 'Z':
// 			case 'W':
// 			case 'L':
// 			case 'H':
// 			case '-':
// 				net->value_to_force[i] = c;
// 				break;
// 			case 'u':
// 				net->value_to_force[i] = 'U';
// 				break;
// 			case 'x':
// 				net->value_to_force[i] = 'X';
// 				break;
// 			case 'z':
// 				net->value_to_force[i] = 'Z';
// 				break;
// 			case 'w':
// 				net->value_to_force[i] = 'W';
// 				break;
// 			case 'l':
// 				net->value_to_force[i] = 'L';
// 				break;
// 			case 'h':
// 				net->value_to_force[i] = 'H';
// 				break;
// 			default:
// 				valid = false;
// 		}
// 	}
// 	if (valid)
// 		m_virtual_board->send_message_to_vpi(VBMessage::write_net(net));
// }
// 
// 
// void InspectorWindow::put_decimal_value(const Glib::ustring& user_string, ModuleNet *net)
// {
// 	(void)user_string;
// 	(void)net;
// }
// 
// 
// void InspectorWindow::put_hexa_value(const Glib::ustring& user_string, ModuleNet *net)
// {
// 	(void)user_string;
// 	(void)net;
// }


