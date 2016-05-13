/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * tonechooser.cc
 * Copyright (C) 2016 Guido Aulisi <guido.aulisi@gmail.com>
 *
 * pg800 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pg800 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_DEBUG
#include <iostream>
#endif

#include "tonechooser.h"

ToneChooser::ToneChooser(mks70_bulkdump * dump)
	: Gtk::Dialog("Bulk tone chooser", true)
{
	Gtk::TreeModel::iterator it;
	Gtk::TreeModel::Row row;

	this->dump = dump;

	set_default_size(300, 240);
	set_border_width(6);
	add_button("Ok", Gtk::RESPONSE_OK);
	add_button("Cancel", Gtk::RESPONSE_CANCEL);

	refListStore = Gtk::ListStore::create(m_Columns);
	// Fill data
	for (unsigned short i = 0; i < mks70_bulkdump::TONE_NUMBER; ++i) {
		it = refListStore->append();
		row = *it;
		row[m_Columns.m_col_number] = i + 1;
		row[m_Columns.m_col_text] = dump->get_tone_name(i);
	}
	tree_view.set_model(refListStore);
	tree_view.append_column("N°", m_Columns.m_col_number);
	tree_view.append_column("Tone name", m_Columns.m_col_text);
	refTreeSelection = tree_view.get_selection();
	refTreeSelection->signal_changed().connect(
		sigc::mem_fun(*this, &ToneChooser::on_selection_changed));

	//Add the TreeView, inside a ScrolledWindow
	m_ScrolledWindow.add(tree_view);

	//Only show the scrollbars when they are necessary:
	m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	area = get_content_area();
	area->pack_start(m_ScrolledWindow);

	show_all_children();
}

void ToneChooser::on_selection_changed()
{
	Gtk::TreeModel::iterator iter = refTreeSelection->get_selected();
	if(iter) {
		Gtk::TreeModel::Row row = *iter;
		tone_number = row.get_value(m_Columns.m_col_number) - 1;
#ifdef HAVE_DEBUG
		std::cout << "Selected tone: " << tone_number << std::endl;
#endif
	}
}
