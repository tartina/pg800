/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * tonechooser.h
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

#ifndef _TONECHOOSER_H_
#define _TONECHOOSER_H_

#include <gtkmm.h>

#include "tonemodelcolumns.h"
#include "bulkdump.h"

class ToneChooser: public Gtk::Dialog
{
public:
	ToneChooser(mks70_bulkdump *dump);
	unsigned short get_tone_number() {return tone_number;}

protected:
	void on_selection_changed();

private:
	unsigned short tone_number;
	Gtk::Box *area;
	ToneModelColumns m_Columns;
	Glib::RefPtr<Gtk::ListStore> refListStore;
	Gtk::TreeView tree_view;
	Gtk::ScrolledWindow m_ScrolledWindow;
	Glib::RefPtr<Gtk::TreeSelection> refTreeSelection;

	mks70_bulkdump *dump;
};

#endif // _TONECHOOSER_H_
