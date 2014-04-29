/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * preferences.h
 * Copyright (C) 2014 Guido Aulisi <guido.aulisi@gmail.com>
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

#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

#include <gtkmm/dialog.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/box.h>
#include <vector>
#include <string>

class preferences: public Gtk::Dialog 
{
public:
	preferences(const std::vector<std::string>& midi_port_name,
                unsigned int midi_port_number = 0,
                unsigned short midi_channel = 0,
	            unsigned short tone_number = 0);
	virtual ~preferences();
	unsigned int get_midi_port_number() {return midi_port_number;};
	unsigned short get_midi_channel() {return midi_channel;};
	unsigned short get_tone_number() {return tone_number;};

private:
	unsigned int midi_port_number = 0;
	unsigned short midi_channel = 0;
	unsigned short tone_number = 0;
	Glib::RefPtr<Gtk::Adjustment> adj_midi_channel;
	Gtk::SpinButton sb_midi_channel;
	Gtk::Label lb_midi_channel, lb_midi_port, lb_tone_number;
	Gtk::Box *area = 0;
	Gtk::ComboBoxText cb_midi_port, cb_tone_number;

	void on_midi_channel_value_changed();
	void on_midi_port_changed();
	void on_tone_number_changed();
};

#endif // _PREFERENCES_H_
