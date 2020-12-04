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

#ifndef INSPECTOR_WINDOW_HH
#define INSPECTOR_WINDOW_HH

#include <gtkmm/window.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/paned.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>
#include <gtkmm/menu.h>
#include <gtkmm/menuitem.h>
//#include <gtkmm/frame.h>
#include "Instance.hh"


class VirtualBoard;


/* Tree model columns for nets */
class NetModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
	NetModelColumns() {
		add(m_col_name);
		add(m_col_width);
		add(m_col_type);
		add(m_col_net);
		add(m_col_format);
		add(m_col_dumy);
	}

	Gtk::TreeModelColumn<std::string> m_col_name;
	Gtk::TreeModelColumn<int>         m_col_width;
	Gtk::TreeModelColumn<std::string> m_col_type;
	Gtk::TreeModelColumn<ModuleNet*>  m_col_net;
	Gtk::TreeModelColumn<char>        m_col_format; // 0: bin, 1: decimal, 2: hexa
	Gtk::TreeModelColumn<bool>        m_col_dumy;
};

class SignalTreeView : public Gtk::TreeView {
private:
	Gtk::MenuItem m_binary_menuitem;
	Gtk::MenuItem m_hexa_menuitem;
	Gtk::MenuItem m_decimal_menuitem;
	Gtk::Menu     m_menu;
	const NetModelColumns *m_model_columns;

protected:
	bool on_button_press_event(GdkEventButton *button_event) override;
	void on_binary_selected();
	void on_hexa_selected();
	void on_decimal_selected();

public:
	SignalTreeView();
	virtual ~SignalTreeView();

	void set_column_model(const NetModelColumns& model_columns);
};




class InspectorWindow : public Gtk::Window {
public:
	/* Tree model columns for modules */
	class ModuleModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		ModuleModelColumns() {
			add(m_col_name);
			add(m_col_net_model);
			add(m_col_module_instance);
		}

		Gtk::TreeModelColumn<std::string>                  m_col_name;
		Gtk::TreeModelColumn<Glib::RefPtr<Gtk::ListStore>> m_col_net_model;
		Gtk::TreeModelColumn<ModuleInstance*>              m_col_module_instance;
	};

private:
	VirtualBoard       *m_virtual_board;
	Gtk::HeaderBar      m_headerbar;
	Gtk::Paned          m_paned;
//	Gtk::Frame          m_frame_modules;
//	Gtk::Frame          m_frame_nets;
	Gtk::ScrolledWindow m_scrollwindow_modules;
	Gtk::ScrolledWindow m_scrollwindow_nets;

	NetModelColumns              m_net_model_column;
	SignalTreeView               m_net_treeview;
	Gtk::CellRendererText        m_net_value_renderer;
	Gtk::TreeView::Column        m_net_value_treeviewcolumn;

	ModuleModelColumns           m_module_model_column;
	Gtk::TreeView                m_module_treeview;
	Glib::RefPtr<Gtk::TreeStore> m_module_ref_tree_model;

	ModuleInstance              *m_curren_module;
	sigc::connection             m_timeout_connection;

public:
	InspectorWindow(VirtualBoard *vb);

	void update_net_row(const ModuleNet& mn);

private:
	bool on_my_delete_event(GdkEventAny* any_event);
	void on_module_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
//	void on_net_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	void build_module_hierarchy_model(Gtk::TreeModel::Row& row, ModuleInstance& inst);

	void treeviewcolumn_net_value_on_cell_data(Gtk::CellRenderer *renderer, const Gtk::TreeModel::iterator& iter);
	void on_my_show();
	void on_my_hide();
	bool on_my_timeout();
};


#endif /* INSPECTOR_WINDOW_HH */
