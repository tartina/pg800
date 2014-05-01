/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * tone.cc
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
#include <iomanip> 
#else
#define NDEBUG
#endif

#include <vector>
#include <cassert>
#include "tone.h"

mks70_tone::mks70_tone()
{
	unsigned short i;

	// Initialize the tone with default values
	name = "INIT TONE";

	for (i = 0; i < 2; i++) {
		dco_range[i] = 1; // dco range = 8'
		dco_wave[i] = 1; // dco wave = saw
		dco_tune[i] = 64; // dco tune
		dco_lfo[i] = 0;
		dco_env[i] = 0;
		mix_dco[i] = 0;
	}
	dco2_xmod = 0;
	dco2_ftune = 64;
	dco_dynamics = 0;
	dco_mode = 3;
	mix_env = 0;

#ifdef HAVE_DEBUG
	dump_tone();
#endif
}

mks70_tone::~mks70_tone() {}

void mks70_tone::set_dco_range(unsigned short dco, unsigned short range,
                               unsigned short midi_channel, RtMidiOut* midi_out, bool send)
{
	if (dco > 1 || range > 3) return;

	if (range != dco_range[dco]) {
		dco_range[dco] = range;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(dco) {
				case 0:
					message.push_back(11);
					break;
				case 1:
					message.push_back(16);
					break;
			}
			message.push_back(range * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO range: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// TODO send dco_wave via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco_wave(unsigned short dco, unsigned short wave,
                              unsigned short midi_channel, RtMidiOut* midi_out, bool send)
{
	if (dco > 1 || wave > 3) return;

	if (wave != dco_wave[dco]) {
		dco_wave[dco] = wave;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(dco) {
				case 0:
					message.push_back(12);
					break;
				case 1:
					message.push_back(17);
					break;
			}
			message.push_back(wave * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO wave: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// TODO send dco_wave via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco2_crossmod(unsigned short crossmod, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (crossmod > 3) return;
	if (crossmod != dco2_xmod) {
		dco2_xmod = crossmod;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(18);
			message.push_back(crossmod * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO2 XMOD: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// TODO send dco_wave via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco_tune(unsigned short dco, unsigned short tune,
                              unsigned short midi_channel, RtMidiOut* midi_out,
                              bool send)
{
	if (dco > 1 || tune > 127) return;

	if (tune != dco_tune[dco]) {
		dco_tune[dco] = tune;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(dco) {
				case 0:
					message.push_back(13);
					break;
				case 1:
					message.push_back(19);
					break;
			}
			message.push_back(tune);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO tune: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// Send dco tune via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco2_ftune(unsigned short ftune, unsigned short midi_channel,
                                RtMidiOut* midi_out, bool send)
{
	if (ftune > 127) return;

	if (ftune != dco2_ftune) {
		dco2_ftune = ftune;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(20);
			message.push_back(ftune);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO2 ftune: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// Send dco tune via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco_lfo(unsigned short dco, unsigned short lfo,
                              unsigned short midi_channel, RtMidiOut* midi_out,
                              bool send)
{
	if (dco > 1 || lfo > 127) return;

	if (lfo != dco_lfo[dco]) {
		dco_lfo[dco] = lfo;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(dco) {
				case 0:
					message.push_back(14);
					break;
				case 1:
					message.push_back(21);
					break;
				default:
					// Error if here
					assert(0);
					break;
			}
			message.push_back(lfo);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO lfo: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// Send dco tune via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco_envelope(unsigned short dco, unsigned short envelope,
                              unsigned short midi_channel, RtMidiOut* midi_out,
                              bool send)
{
	if (dco > 1 || envelope > 127) return;

	if (envelope != dco_env[dco]) {
		dco_env[dco] = envelope;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(dco) {
				case 0:
					message.push_back(15);
					break;
				case 1:
					message.push_back(22);
					break;
				default:
					// Error if here
					assert(0);
					break;
			}
			message.push_back(envelope);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO lfo: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// Send dco tune via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco_dyna(unsigned short dyna, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (dyna > 3) return;
	if (dyna != dco_dynamics) {
		dco_dynamics = dyna;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(26);
			message.push_back(dyna * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO DYNA: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// TODO send dco_wave via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco_mode(unsigned short mode, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (mode > 3) return;
	if (mode != dco_mode) {
		dco_mode = mode;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(27);
			message.push_back(mode * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO MODE: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// TODO send dco_wave via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_mixer_dco(unsigned short dco, unsigned short value,
                              unsigned short midi_channel, RtMidiOut* midi_out,
                              bool send)
{
	if (dco > 1 || value > 127) return;

	if (value != mix_dco[dco]) {
		mix_dco[dco] = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(dco) {
				case 0:
					message.push_back(28);
					break;
				case 1:
					message.push_back(29);
					break;
				default:
					// Error if here
					assert(0);
					break;
			}
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Mixer DCO: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// Send dco tune via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_mixer_envelope(unsigned short envelope, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (envelope > 127) return;

	if (envelope != mix_env) {
		mix_env = envelope;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(30);
			message.push_back(envelope);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Mixer env: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << " ";
			std::cout << std::endl;
#endif
			// Send dco tune via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_name(const std::string& name)
{
	this->name = name.substr(0 ,10);
}

void mks70_tone::set_tone_number(const unsigned short tone)
{
	if (tone < 2) tone_number = tone;
}

#ifdef HAVE_DEBUG
void mks70_tone::dump_tone()
{
	unsigned short i;

	std::cout << "Name: " << name << std::endl;

	for (i = 0; i < 2; i++) {
		std::cout << std::setbase(10) << "DCO" << (i + 1) << " Range: " << dco_range[i] << std::endl;
		std::cout << std::setbase(10) << "DCO" << (i + 1) << " Waveform: " << dco_wave[i] << std::endl;
		std::cout << std::setbase(10) << "DCO" << (i + 1) << " Tune: " << dco_tune[i] << std::endl;
		std::cout << std::setbase(10) << "DCO" << (i + 1) << " Lfo: " << dco_lfo[i] << std::endl;
		std::cout << std::setbase(10) << "DCO" << (i + 1) << " Envelope: " << dco_env[i] << std::endl;
		std::cout << std::setbase(10) << "Mixer DCO" << (i + 1) << ": " << mix_dco[i] << std::endl;
	}
	std::cout << std::setbase(10) << "DCO2 XMod: " << dco2_xmod << std::endl;
	std::cout << std::setbase(10) << "DCO2 FTune: " << dco2_ftune << std::endl;
	std::cout << std::setbase(10) << "DCO Dyna: " << dco_dynamics << std::endl;
	std::cout << std::setbase(10) << "DCO Mode: " << dco_mode << std::endl;
	std::cout << std::setbase(10) << "Mixer envelope: " << mix_env << std::endl;
}
#endif

const std::string mks70_tone::init_tone_name = "INIT TONE";