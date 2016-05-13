/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * tonemodelcolumn.h
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

#ifndef _TONEMODELCOLUMN_H_
#define _TONEMODELCOLUMN_H_

#include <gtkmm.h>

class ToneModelColumns : public Gtk::TreeModelColumnRecord
{
public:

	ToneModelColumns() { add(m_col_number); add(m_col_text); }

	Gtk::TreeModelColumn<int> m_col_number;
	Gtk::TreeModelColumn<Glib::ustring> m_col_text;
};

#endif // _TONEMODELCOLUMN_H_
