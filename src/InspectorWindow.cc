#include "InspectorWindow.hh"
#include "virtual_board.hh"
//#include <iostream>


InspectorWindow::InspectorWindow(VirtualBoard* vb) :
	Gtk::Window(),
	m_virtual_board(vb),
	m_headerbar(),
	m_paned(),
//	m_frame_modules(),
//	m_frame_nets(),
	m_scrollwindow_modules(),
	m_scrollwindow_nets()
{
	set_icon_name("gtk-find");
	m_headerbar.set_title("Signal inspector");
	m_headerbar.set_show_close_button();
	set_titlebar(m_headerbar);
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
	m_module_treeview.set_reorderable();
	m_module_treeview.set_enable_tree_lines();
	m_module_treeview.set_show_expanders();

	m_module_treeview.set_model(m_module_ref_tree_model);
	m_module_treeview.expand_all();
	m_module_treeview.set_activate_on_single_click();

	//Connect signal:
	m_module_treeview.signal_row_activated().connect(sigc::mem_fun(*this, &InspectorWindow::on_module_treeview_row_activated));





	/* NETs */

	/* Add the TreeView's view columns */
	m_net_treeview.append_column("Nets",  m_net_model_column.m_col_name);
	m_net_treeview.append_column("Width", m_net_model_column.m_col_width);
	m_net_treeview.append_column("IO",    m_net_model_column.m_col_type);
	m_net_treeview.append_column("Value", m_net_model_column.m_col_value);
	m_module_treeview.set_reorderable();
	m_net_treeview.set_rules_hint();
	m_net_treeview.set_rubber_banding();
	//m_net_treeview.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH); //TREE_VIEW_GRID_LINES_HORIZONTAL

	//m_net_treeview.append_column_numeric("Formatted number", m_net_model_column.m_col_number,
	//		"%010d" /* 10 digits, using leading zeroes. */);

	//Make all the columns reorderable:
	//This is not necessary, but it's nice to show the feature.
	//You can use TreeView::set_column_drag_function() to more
	//finely control column drag and drop.
	for (guint i = 0; i < 4; i++) {
		m_net_treeview.get_column(i)->set_reorderable();
		m_net_treeview.get_column(i)->set_resizable();
	}

	//m_net_treeview.columns_autosize();
	//m_net_treeview.check_resize();
	
	Gtk::TreeModel::iterator iter = m_module_ref_tree_model->get_iter("0");
	if (iter) {
		row = *iter;
		Glib::RefPtr<Gtk::ListStore> top_module_net_list = row[m_module_model_column.m_col_net_model];
		m_net_treeview.set_model(top_module_net_list);
	}
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
	}
}


void InspectorWindow::build_module_hierarchy_model(Gtk::TreeModel::Row& module_row, const ModuleInstance& inst)
{
	int i, s;
	Glib::RefPtr<Gtk::ListStore> net_ref_tree_model = Gtk::ListStore::create(m_net_model_column);

	s = inst.nets.size();
	for (i = 0; i < s; i++) {
		Gtk::TreeModel::Row net_row = *(net_ref_tree_model->append());
		net_row[m_net_model_column.m_col_name] = inst.nets[i].name;
		net_row[m_net_model_column.m_col_width] = inst.nets[i].width;
		switch (inst.nets[i].direction) {
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
		net_row[m_net_model_column.m_col_value] = std::string((size_t)inst.nets[i].width, 'U');
	}

	module_row[m_module_model_column.m_col_name] = inst.name;
	module_row[m_module_model_column.m_col_net_model] = net_ref_tree_model;

	s = inst.modules.size();
	for (i = 0; i < s; i++) {
		Gtk::TreeModel::Row childrow = *(m_module_ref_tree_model->append(module_row.children()));
		build_module_hierarchy_model(childrow, inst.modules[i]);
	}
}


