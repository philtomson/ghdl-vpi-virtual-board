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
//#include <gtkmm/frame.h>
#include "Instance.hh"

class VirtualBoard;


class InspectorWindow : public Gtk::Window {
protected:
	/* Tree model columns for nets */
	class NetModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		NetModelColumns() {
			add(m_col_name);
			add(m_col_width);
			add(m_col_type);
			add(m_col_value);
		}

		Gtk::TreeModelColumn<std::string> m_col_name;
		Gtk::TreeModelColumn<int> m_col_width;
		Gtk::TreeModelColumn<std::string> m_col_type;
		Gtk::TreeModelColumn<std::string> m_col_value;
	};

	/* Tree model columns for modules */
	class ModuleModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		ModuleModelColumns() {
			add(m_col_name);
			add(m_col_net_model);
		}

		Gtk::TreeModelColumn<std::string> m_col_name;
		Gtk::TreeModelColumn<Glib::RefPtr<Gtk::ListStore>> m_col_net_model;
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
	Gtk::TreeView                m_net_treeview;

	ModuleModelColumns           m_module_model_column;
	Gtk::TreeView                m_module_treeview;
	Glib::RefPtr<Gtk::TreeStore> m_module_ref_tree_model;

public:
	InspectorWindow(VirtualBoard *vb);

private:
	bool on_my_delete_event(GdkEventAny* any_event);
	void on_module_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	void build_module_hierarchy_model(Gtk::TreeModel::Row& row, const ModuleInstance& inst);
};


#endif /* INSPECTOR_WINDOW_HH */
