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

preferences::preferences(const std::vector<std::string>& midi_port_name,
                         const unsigned int midi_port_number,
                         const unsigned short midi_channel,
                         const unsigned short tone_number,
                         const std::string tone_name)
	: Gtk::Dialog("Preferences", true),
	adj_midi_channel(Gtk::Adjustment::create(midi_channel + 1, 1.0, 16.0, 1.0, 1.0, 0.0)),
	sb_midi_channel(adj_midi_channel),
	lb_midi_channel("MIDI channel"),
	lb_midi_port("MIDI port"),
	lb_tone_number("Tone"),
	lb_tone_name("Tone name")
{
	this->midi_port_number = 0;
	this->midi_channel = 0;
	this->tone_number = 0;
	this->tone_name.clear();
	this->area = 0;

	if (midi_channel < 16) this->midi_channel = midi_channel;
	this->midi_port_number = midi_port_number;
	if (tone_number < 2) this->tone_number = tone_number;
	if (tone_name.length() > 0) this->tone_name = tone_name;

	set_border_width(6);
	add_button("Ok", Gtk::RESPONSE_OK);
	add_button("Cancel", Gtk::RESPONSE_CANCEL);
	sb_midi_channel.set_numeric();
	sb_midi_channel.signal_value_changed().connect( sigc::mem_fun(*this,
		&preferences::on_midi_channel_value_changed) );

	for (std::vector<std::string>::const_iterator it = midi_port_name.begin();
			it != midi_port_name.end(); ++it)
		cb_midi_port.append(*it);
	cb_midi_port.set_active(midi_port_number);

	cb_midi_port.signal_changed().connect(sigc::mem_fun(*this,
		&preferences::on_midi_port_changed));

	cb_tone_number.append("Tone A");
	cb_tone_number.append("Tone B");
	cb_tone_number.set_active(tone_number);
	cb_tone_number.signal_changed().connect(sigc::mem_fun(*this,
		&preferences::on_tone_number_changed));

	en_tone_name.set_max_length(10);
	en_tone_name.set_text(tone_name);
	en_tone_name.signal_changed().connect(sigc::mem_fun(*this,
		&preferences::on_tone_name_changed));

	area = get_content_area();
	area->pack_start(lb_midi_channel, Gtk::PACK_SHRINK);
	area->pack_start(sb_midi_channel, Gtk::PACK_SHRINK);
	area->pack_start(lb_midi_port, Gtk::PACK_SHRINK);
	area->pack_start(cb_midi_port, Gtk::PACK_SHRINK);
	area->pack_start(lb_tone_number, Gtk::PACK_SHRINK);
	area->pack_start(cb_tone_number, Gtk::PACK_SHRINK);
	area->pack_start(lb_tone_name, Gtk::PACK_SHRINK);
	area->pack_start(en_tone_name, Gtk::PACK_SHRINK);

	show_all_children();
}

preferences::~preferences() {}

void preferences::on_midi_channel_value_changed()
{
	midi_channel = sb_midi_channel.get_value() - 1;
	if (midi_channel > 15) midi_channel = 0;
#ifdef HAVE_DEBUG
	std::cout << "MIDI channel: " << midi_channel << std::endl;
#endif
}

void preferences::on_midi_port_changed()
{
	midi_port_number = cb_midi_port.get_active_row_number();
#ifdef HAVE_DEBUG
	std::cout << "MIDI port: " << midi_port_number << std::endl;
#endif
}

void preferences::on_tone_number_changed()
{
	tone_number = cb_tone_number.get_active_row_number();
#ifdef HAVE_DEBUG
	std::cout << "Tone number: " << tone_number << std::endl;
#endif
}

void preferences::on_tone_name_changed()
{
	tone_name = en_tone_name.get_text();
#ifdef HAVE_DEBUG
	std::cout << "Tone name: " << tone_name << std::endl;
#endif
}