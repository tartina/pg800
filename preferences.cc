/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * preferences.cc
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_DEBUG
#include <iostream>
#endif

#include "preferences.h"

preferences::preferences(const std::vector<std::string>& midi_port_name)
	: Gtk::Dialog("Preferences", true),
	adj_midi_channel(Gtk::Adjustment::create(1.0, 1.0, 16.0, 1.0, 1.0, 0.0)),
	sb_midi_channel(adj_midi_channel),
	lb_midi_channel("MIDI channel"),
	lb_midi_port("MIDI port")
{
	set_border_width(6);
	add_button("Ok", Gtk::ResponseType::RESPONSE_OK);
	add_button("Cancel", Gtk::ResponseType::RESPONSE_CANCEL);
	sb_midi_channel.set_numeric();
	sb_midi_channel.signal_value_changed().connect( sigc::mem_fun(*this,
		&preferences::on_midi_channel_value_changed) );

	for (std::vector<std::string>::const_iterator it = midi_port_name.begin();
			it != midi_port_name.end(); ++it)
		cb_midi_port.append(*it);

	cb_midi_port.signal_changed().connect( sigc::mem_fun(*this,
		&preferences::on_midi_port_changed) );

	area = get_content_area();
	area->pack_start(lb_midi_channel, Gtk::PACK_SHRINK);
	area->pack_start(sb_midi_channel, Gtk::PACK_SHRINK);
	area->pack_start(lb_midi_port, Gtk::PACK_SHRINK);
	area->pack_start(cb_midi_port, Gtk::PACK_SHRINK);

	show_all_children();
}

preferences::~preferences() {}

void preferences::on_midi_channel_value_changed()
{
	midi_channel = sb_midi_channel.get_value() - 1;
	if (midi_channel > 15) midi_channel = 0;
#ifdef HAVE_DEBUG
	std::cout << "MIDI channel: " << midi_channel << "\n";
#endif
}

void preferences::on_midi_port_changed()
{
	midi_port_number = cb_midi_port.get_active_row_number();
#ifdef HAVE_DEBUG
	std::cout << "MIDI port: " << midi_port_number << "\n";
#endif
}