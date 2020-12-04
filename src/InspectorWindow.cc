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
	bool ret = false;

	ret = TreeView::on_button_press_event(button_event);

	if ((button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3))
		m_menu.popup_at_pointer((GdkEvent*)button_event);

	return ret;
}


void SignalTreeView::on_binary_selected()
{
	Glib::RefPtr<Gtk::TreeSelection> ref_selection = get_selection();
	if (ref_selection) {
		Gtk::TreeModel::iterator iter = ref_selection->get_selected();
		if (iter) {
			(*iter)[m_model_columns->m_col_format] = 0;
		}
	}
}


void SignalTreeView::on_hexa_selected()
{
	Glib::RefPtr<Gtk::TreeSelection> ref_selection = get_selection();
	if (ref_selection) {
		Gtk::TreeModel::iterator iter = ref_selection->get_selected();
		if (iter) {
			(*iter)[m_model_columns->m_col_format] = 2;
		}
	}
}


void SignalTreeView::on_decimal_selected()
{
	Glib::RefPtr<Gtk::TreeSelection> ref_selection = get_selection();
	if (ref_selection) {
		Gtk::TreeModel::iterator iter = ref_selection->get_selected();
		if (iter) {
			(*iter)[m_model_columns->m_col_format] = 1;
		}
	}
}


InspectorWindow::InspectorWindow(VirtualBoard* vb) :
	Gtk::Window(),
	m_virtual_board(vb),
	m_headerbar(),
	m_paned(),
//	m_frame_modules(),
//	m_frame_nets(),
	m_scrollwindow_modules(),
	m_scrollwindow_nets(),
	m_net_value_renderer(),
	m_net_value_treeviewcolumn(),
	m_curren_module(nullptr),
	m_timeout_connection()
{
	set_icon_name("gtk-find");
	m_headerbar.set_title("Signal inspector");
	m_headerbar.set_show_close_button();
	set_titlebar(m_headerbar);
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
#if 0
	m_frame_modules.add(m_scrollwindow_modules);
	m_frame_modules.set_shadow_type(Gtk::SHADOW_IN);
	m_frame_nets.add(m_scrollwindow_nets);
	m_frame_nets.set_shadow_type(Gtk::SHADOW_IN);
	//m_frame_modules.set_margin_left(0);
	//m_frame_modules.set_margin_right(0);
	//m_frame_nets.set_margin_left(0);
	//m_frame_nets.set_margin_right(0);
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
	/*******************************************************/
	m_net_value_treeviewcolumn.set_title("Value");
	m_net_value_treeviewcolumn.pack_start(m_net_value_renderer);
	m_net_treeview.append_column(m_net_value_treeviewcolumn);
	//Tell the view column how to render the model values:
	m_net_value_treeviewcolumn.set_cell_data_func(m_net_value_renderer,
			sigc::mem_fun(*this, &InspectorWindow::treeviewcolumn_net_value_on_cell_data));
	m_net_value_renderer.property_family() = "Monospace";
	//m_net_value_renderer.property_style() = Pango::STYLE_ITALIC;

//	//Make the CellRenderer editable, and handle its editing signals:
//	m_net_value_renderer.property_editable() = true;
//
//	m_net_value_renderer.signal_editing_started().connect(
//			sigc::mem_fun(*this,
//				&InspectorWindow::cellrenderer_net_value_on_editing_started) );
//
//	m_net_value_renderer.signal_edited().connect(sigc::mem_fun(*this,
//				&InspectorWindow::cellrenderer_net_value_on_edited));
	/*******************************************************/
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

	m_net_treeview.set_rules_hint();
	//m_net_treeview.set_rubber_banding();
	//m_net_treeview.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH); //TREE_VIEW_GRID_LINES_HORIZONTAL
	m_net_treeview.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_VERTICAL); //TREE_VIEW_GRID_LINES_HORIZONTAL

	//m_net_treeview.append_column_numeric("Formatted number", m_net_model_column.m_col_number,
	//		"%010d" /* 10 digits, using leading zeroes. */);

	//Make all the columns reorderable:
	//This is not necessary, but it's nice to show the feature.
	//You can use TreeView::set_column_drag_function() to more
	//finely control column drag and drop.
	for (guint i = 0; i < 4; i++) {
		m_net_treeview.get_column(i)->set_reorderable();
		m_net_treeview.get_column(i)->set_resizable();
		//m_net_treeview.get_column(i)->set_sizing(Gtk::TREE_VIEW_COLUMN_AUTOSIZE);
		m_net_treeview.get_column(i)->set_expand(false);
	}

	//m_net_treeview.columns_autosize();
	//m_net_treeview.check_resize();
	
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


void InspectorWindow::update_net_row(const ModuleNet& mn)
{
	Gtk::TreeModel::Row net_row = *(mn.row);
	net_row[m_net_model_column.m_col_dumy] = net_row[m_net_model_column.m_col_dumy] xor true;
}


void InspectorWindow::build_module_hierarchy_model(Gtk::TreeModel::Row& module_row, ModuleInstance& inst)
{
	int i, s;
	Glib::RefPtr<Gtk::ListStore> net_ref_tree_model = Gtk::ListStore::create(m_net_model_column);

	for (std::vector<ModuleNet>::iterator it = inst.nets.begin(); it != inst.nets.end(); ++it) {
		it->row = net_ref_tree_model->append();
		Gtk::TreeModel::Row net_row = *(it->row);
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
				m_net_value_renderer.property_text() = "Decimal";
				break;
			case 2:
				m_net_value_renderer.property_text() = "Hexa";
				break;
			default:
				m_net_value_renderer.property_text() = modnet->value;
		}
		//m_net_value_renderer.property_text() = std::string("Coucou les amis");
	}
}


// void InspectorWindow::cellrenderer_net_value_on_editing_started(
//         Gtk::CellEditable* cell_editable, const Glib::ustring& path)
// {
//   //Start editing with previously-entered (but invalid) text,
//   //if we are allowing the user to correct some invalid data.
//   if(m_validate_retry)
//   {
//     //This is the CellEditable inside the CellRenderer.
//     auto celleditable_validated = cell_editable;
// 
//     //It's usually an Entry, at least for a CellRendererText:
//     auto pEntry = dynamic_cast<Gtk::Entry*>(celleditable_validated);
//     if(pEntry)
//     {
//       pEntry->set_text(m_invalid_text_for_retry);
//       m_validate_retry = false;
//       m_invalid_text_for_retry.clear();
//     }
//   }
// 
// }
// 
// 
// void InspectorWindow::cellrenderer_net_value_on_edited(
//         const Glib::ustring& path_string,
//         const Glib::ustring& new_text)
// {
//   Gtk::TreePath path(path_string);
// 
//   //Convert the inputed text to an integer, as needed by our model column:
//   char* pchEnd = nullptr;
//   int new_value = strtol(new_text.c_str(), &pchEnd, 10);
// 
//   if(new_value > 10)
//   {
//     //Prevent entry of numbers higher than 10.
// 
//     //Tell the user:
//     Gtk::MessageDialog dialog(*this,
//             "The number must be less than 10. Please try again.",
//             false, Gtk::MESSAGE_ERROR);
//     dialog.run();
// 
//     //Start editing again, with the bad text, so that the user can correct it.
//     //A real application should probably allow the user to revert to the
//     //previous text.
// 
//     //Set the text to be used in the start_editing signal handler:
//     m_invalid_text_for_retry = new_text;
//     m_validate_retry = true;
// 
//     //Start editing again:
//     m_TreeView.set_cursor(path, m_treeviewcolumn_validated,
//             m_cellrenderer_validated, true /* start_editing */);
//   }
//   else
//   {
//     //Get the row from the path:
//     Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
//     if(iter)
//     {
//       Gtk::TreeModel::Row row = *iter;
// 
//       //Put the new value in the model:
//       row[m_Columns.m_col_number_validated] = new_value;
//     }
//   }
// }


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


