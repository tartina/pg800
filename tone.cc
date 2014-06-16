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
#include <libxml++/libxml++.h>

#ifdef HAVE_BOOST_LEXICAL_CAST_HPP
#include <boost/lexical_cast.hpp>
#else
#error We need boost lexical_cast
#endif

#include "tone.h"

mks70_tone::mks70_tone()
{
	init_tone();
}

void mks70_tone::init_tone()
{
	unsigned short i;

	// Initialize the tone with default values
	name = "INIT TONE";

	for (i = 0; i < 2; i++) {
		dco_range[i] = 1; // dco range = 8'
		dco_wave[i] = 3; // dco wave = saw
		dco_tune[i] = 64; // dco tune
		dco_lfo[i] = 0;
		dco_env[i] = 0;
		mix_dco[i] = 64;
		env_attack_time[i] = 0;
		env_decay_time[i] = 0;
		env_sustain_level[i] = 127;
		env_release_time[i] = 0;
		env_key_follow[i] = 0;
	}

	dco2_xmod = 0;
	dco2_ftune = 64;
	dco_dynamics = 0;
	dco_mode = 3;
	mix_env = 0;
	mix_dynamics = 0;
	mix_env_mode = 3;
	vcf_hpf = 0;
	vcf_cutoff_freq = 127;
	vcf_resonance = 0;
	vcf_lfo_mod_depth = 0;
	vcf_env_mod_depth = 0;
	vcf_key_follow = 0;
	vcf_lfo_mod_depth = 0;
	vcf_env_mod_depth = 0;
	vcf_key_follow = 0;
	vcf_dynamics = 0;
	vcf_env_mode = 3;
	vca_level = 64;
	vca_dynamics = 0;
	vca_env_mode = 1;
	lfo_waveform = 2;
	lfo_delay_time = 0;
	lfo_rate = 0;
	chorus = 0;

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

void mks70_tone::set_mixer_dyna(unsigned short dyna, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (dyna > 3) return;
	if (dyna != mix_dynamics) {
		mix_dynamics = dyna;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(31);
			message.push_back(dyna * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Mixer DYNA: ";
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

void mks70_tone::set_mixer_mode(unsigned short mode, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (mode > 3) return;
	if (mode != mix_env_mode) {
		mix_env_mode = mode;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(32);
			message.push_back(mode * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Mixer MODE: ";
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

void mks70_tone::set_vcf_hpf(unsigned short value, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (value > 3) return;
	if (value != vcf_hpf) {
		vcf_hpf = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(33);
			message.push_back(value * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCF HPF: ";
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

void mks70_tone::set_vcf_cutoff(unsigned short value, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (value > 127) return;

	if (value != vcf_cutoff_freq) {
		vcf_cutoff_freq = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(34);
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCF Cutoff: ";
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

void mks70_tone::set_vcf_resonance(unsigned short value, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (value > 127) return;

	if (value != vcf_resonance) {
		vcf_resonance = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(35);
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCF Resonance: ";
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

void mks70_tone::set_vcf_lfo(unsigned short value, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (value > 127) return;

	if (value != vcf_lfo_mod_depth) {
		vcf_lfo_mod_depth = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(36);
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCF LFO: ";
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

void mks70_tone::set_vcf_envelope(unsigned short value, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (value > 127) return;

	if (value != vcf_env_mod_depth) {
		vcf_env_mod_depth = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(37);
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCF Envelope: ";
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

void mks70_tone::set_vcf_key(unsigned short value, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (value > 127) return;

	if (value != vcf_key_follow) {
		vcf_key_follow = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(38);
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCF Key Follow: ";
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

void mks70_tone::set_vcf_dyna(unsigned short dyna, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (dyna > 3) return;
	if (dyna != vcf_dynamics) {
		vcf_dynamics = dyna;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(39);
			message.push_back(dyna * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCF DYNA: ";
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

void mks70_tone::set_vcf_env_mode(unsigned short mode, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (mode > 3) return;
	if (mode != vcf_env_mode) {
		vcf_env_mode = mode;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(40);
			message.push_back(mode * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCF Envelope MODE: ";
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

void mks70_tone::set_vca_level(unsigned short value, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (value > 127) return;

	if (value != vca_level) {
		vca_level = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(41);
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCA Level: ";
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

void mks70_tone::set_vca_dyna(unsigned short dyna, unsigned short midi_channel,
                              RtMidiOut* midi_out, bool send)
{
	if (dyna > 3) return;
	if (dyna != vca_dynamics) {
		vca_dynamics = dyna;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(42);
			message.push_back(dyna * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCA DYNA: ";
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

void mks70_tone::set_vca_env_mode(unsigned short mode, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (mode > 1) return;
	if (mode != vca_env_mode) {
		vca_env_mode = mode;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(58);
			message.push_back(mode * 64);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI VCA Envelope MODE: ";
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

void mks70_tone::set_lfo_waveform(unsigned short waveform, unsigned short midi_channel,
                                   RtMidiOut* midi_out, bool send)
{
	if (waveform > 2) return;
	if (waveform != lfo_waveform) {
		lfo_waveform = waveform;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(44);
			message.push_back(waveform * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI LFO Waveform: ";
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

void mks70_tone::set_lfo_delay_time(unsigned short value, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (value > 127) return;

	if (value != lfo_delay_time) {
		lfo_delay_time = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(45);
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI LFO Delay Time: ";
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

void mks70_tone::set_lfo_rate(unsigned short value, unsigned short midi_channel,
                                    RtMidiOut* midi_out, bool send)
{
	if (value > 127) return;

	if (value != lfo_rate) {
		lfo_rate = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(46);
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI LFO Rate: ";
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

void mks70_tone::set_envelope_attack(unsigned short envelope, unsigned short value,
                              unsigned short midi_channel, RtMidiOut* midi_out,
                              bool send)
{
	if (envelope > 1 || value > 127) return;

	if (value != env_attack_time[envelope]) {
		env_attack_time[envelope] = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(envelope) {
				case 0:
					message.push_back(47);
					break;
				case 1:
					message.push_back(52);
					break;
				default:
					// Error if here
					assert(0);
					break;
			}
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Envelope Attack: ";
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

void mks70_tone::set_envelope_decay(unsigned short envelope, unsigned short value,
                              unsigned short midi_channel, RtMidiOut* midi_out,
                              bool send)
{
	if (envelope > 1 || value > 127) return;

	if (value != env_decay_time[envelope]) {
		env_decay_time[envelope] = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(envelope) {
				case 0:
					message.push_back(48);
					break;
				case 1:
					message.push_back(53);
					break;
				default:
					// Error if here
					assert(0);
					break;
			}
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Envelope Decay: ";
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

void mks70_tone::set_envelope_sustain(unsigned short envelope, unsigned short value,
                              unsigned short midi_channel, RtMidiOut* midi_out,
                              bool send)
{
	if (envelope > 1 || value > 127) return;

	if (value != env_sustain_level[envelope]) {
		env_sustain_level[envelope] = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(envelope) {
				case 0:
					message.push_back(49);
					break;
				case 1:
					message.push_back(54);
					break;
				default:
					// Error if here
					assert(0);
					break;
			}
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Envelope Sustain: ";
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

void mks70_tone::set_envelope_release(unsigned short envelope, unsigned short value,
                              unsigned short midi_channel, RtMidiOut* midi_out,
                              bool send)
{
	if (envelope > 1 || value > 127) return;

	if (value != env_release_time[envelope]) {
		env_release_time[envelope] = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(envelope) {
				case 0:
					message.push_back(50);
					break;
				case 1:
					message.push_back(55);
					break;
				default:
					// Error if here
					assert(0);
					break;
			}
			message.push_back(value);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Envelope Release: ";
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

void mks70_tone::set_envelope_key_follow(unsigned short envelope, unsigned short value,
                               unsigned short midi_channel, RtMidiOut* midi_out, bool send)
{
	if (envelope > 1 || value > 3) return;

	if (value != env_key_follow[envelope]) {
		env_key_follow[envelope] = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			switch(envelope) {
				case 0:
					message.push_back(51);
					break;
				case 1:
					message.push_back(56);
					break;
			}
			message.push_back(value * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Envelope Key Follow: ";
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

void mks70_tone::set_chorus(unsigned short value, unsigned short midi_channel,
                              RtMidiOut* midi_out, bool send)
{
	if (value > 2) return;
	if (value != chorus) {
		chorus = value;
		if (send) {
			message.clear();
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x20);
			message.push_back(tone_number + 1);
			message.push_back(43);
			message.push_back(chorus * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI Chorus: ";
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

void mks70_tone::set_name(const std::string& name)
{
	this->name = name.substr(0 ,10);
}

void mks70_tone::set_tone_number(const unsigned short tone)
{
	if (tone < 2) tone_number = tone;
}

void mks70_tone::apr_send(unsigned short midi_channel, RtMidiOut* midi_out)
{
	std::string::const_iterator it;
	unsigned short i;

	message.clear();
	message.push_back(0xF0);
	message.push_back(0x41);
	message.push_back(0x35);
	message.push_back(midi_channel);
	message.push_back(0x24);
	message.push_back(0x20);
	message.push_back(tone_number + 1);
	// Message: 59 byte
	for (it = name.begin(), i = 0; i < 10 && it < name.end(); ++i, ++it)
		message.push_back(*it);
	for (; i < 10; i++) message.push_back(' ');
	message.push_back(0); // Byte 10 undefined
	message.push_back(dco_range[0] * 32);
	message.push_back(dco_wave[0] * 32);
	message.push_back(dco_tune[0]);
	message.push_back(dco_lfo[0]);
	message.push_back(dco_env[0]);
	message.push_back(dco_range[1] * 32);
	message.push_back(dco_wave[1] * 32);
	message.push_back(dco2_xmod * 32);
	message.push_back(dco_tune[1]);
	message.push_back(dco2_ftune);
	message.push_back(dco_lfo[1]);
	message.push_back(dco_env[1]);
	message.push_back(0); // Byte 23 undefined
	message.push_back(0); // Byte 24 undefined
	message.push_back(0); // Byte 25 undefined
	message.push_back(dco_dynamics * 32);
	message.push_back(dco_mode * 32);
	message.push_back(mix_dco[0]);
	message.push_back(mix_dco[1]);
	message.push_back(mix_env);
	message.push_back(mix_dynamics * 32);
	message.push_back(mix_env_mode * 32);
	message.push_back(vcf_hpf * 32);
	message.push_back(vcf_cutoff_freq);
	message.push_back(vcf_resonance);
	message.push_back(vcf_lfo_mod_depth);
	message.push_back(vcf_env_mod_depth);
	message.push_back(vcf_key_follow);
	message.push_back(vcf_dynamics * 32);
	message.push_back(vcf_env_mode * 32);
	message.push_back(vca_level);
	message.push_back(vca_dynamics * 32);
	message.push_back(chorus * 32);
	message.push_back(lfo_waveform * 32);
	message.push_back(lfo_delay_time);
	message.push_back(lfo_rate);
	for (i = 0; i < 2; i++) {
		message.push_back(env_attack_time[i]);
		message.push_back(env_decay_time[i]);
		message.push_back(env_sustain_level[i]);
		message.push_back(env_release_time[i]);
		message.push_back(env_key_follow[i] * 32);
	}
	message.push_back(0); // Byte 57 undefined
	message.push_back(vca_env_mode * 64);
	message.push_back(0xF7);

#ifdef HAVE_DEBUG
	std::cout << "MIDI APR Send: ";
	for (std::vector<unsigned char>::iterator it = message.begin();
	     it < message.end(); ++it)
		std::cout << std::setbase(16) << (unsigned short)*it << " ";
	std::cout << std::endl;
#endif
	midi_out->sendMessage(&message);
}

void mks70_tone::save_to_file(const std::string& file_name) const
{
	unsigned short i;
	xmlpp::Document* doc = 0;
	xmlpp::Element* root_node = 0;
	xmlpp::Element* current_node = 0;
	xmlpp::Element* xmlnode[2] = {0,};
	xmlpp::Element* elem[2] = {0,};

	doc = new xmlpp::Document();
	root_node = doc->create_root_node("mks70tone", "http://heavyware.it/namespace/mks70",
	                                  "mks70");
	current_node = root_node->add_child("mks70:name");
	current_node->add_child_text(name);

	// DCO
	for (i = 0; i < 2; i++) {
		xmlnode[i] = root_node->add_child("mks70:dco");
		xmlnode[i]->set_attribute("index", std::to_string(i + 1));
		elem[i] = xmlnode[i]->add_child("mks70:range");
		elem[i]->add_child_text(std::to_string(dco_range[i]));
		elem[i] = xmlnode[i]->add_child("mks70:waveform");
		elem[i]->add_child_text(std::to_string(dco_wave[i]));
		elem[i] = xmlnode[i]->add_child("mks70:tune");
		elem[i]->add_child_text(std::to_string(dco_tune[i]));
		elem[i] = xmlnode[i]->add_child("mks70:lfo");
		elem[i]->add_child_text(std::to_string(dco_lfo[i]));
		elem[i] = xmlnode[i]->add_child("mks70:envelope");
		elem[i]->add_child_text(std::to_string(dco_env[i]));
	}
	elem[1] = xmlnode[1]->add_child("mks70:xmod");
	elem[1]->add_child_text(std::to_string(dco2_xmod));
	elem[1] = xmlnode[1]->add_child("mks70:ftune");
	elem[1]->add_child_text(std::to_string(dco2_ftune));
	elem[1] = xmlnode[1]->add_child("mks70:dynamics");
	elem[1]->add_child_text(std::to_string(dco_dynamics));
	elem[1] = xmlnode[1]->add_child("mks70:mode");
	elem[1]->add_child_text(std::to_string(dco_mode));

	// Mixer
	current_node = root_node->add_child("mks70:mixer");
	for (i = 0; i < 2; i++) {
		xmlnode[i] = current_node->add_child("mks70:dco");
		xmlnode[i]->set_attribute("index", std::to_string(i + 1));
		xmlnode[i]->add_child_text(std::to_string(mix_dco[i]));
	}
	elem[0] = current_node->add_child("mks70:envelope");
	elem[0]->add_child_text(std::to_string(mix_env));
	elem[0] = current_node->add_child("mks70:dynamics");
	elem[0]->add_child_text(std::to_string(mix_dynamics));
	elem[0] = current_node->add_child("mks70:mode");
	elem[0]->add_child_text(std::to_string(mix_env_mode));

	// VCF
	current_node = root_node->add_child("mks70:vcf");
	elem[0] = current_node->add_child("mks70:hpf");
	elem[0]->add_child_text(std::to_string(vcf_hpf));
	elem[0] = current_node->add_child("mks70:cutoff");
	elem[0]->add_child_text(std::to_string(vcf_cutoff_freq));
	elem[0] = current_node->add_child("mks70:resonance");
	elem[0]->add_child_text(std::to_string(vcf_resonance));
	elem[0] = current_node->add_child("mks70:lfo");
	elem[0]->add_child_text(std::to_string(vcf_lfo_mod_depth));
	elem[0] = current_node->add_child("mks70:envelope");
	elem[0]->add_child_text(std::to_string(vcf_env_mod_depth));
	elem[0] = current_node->add_child("mks70:key");
	elem[0]->add_child_text(std::to_string(vcf_key_follow));
	elem[0] = current_node->add_child("mks70:dynamics");
	elem[0]->add_child_text(std::to_string(vcf_dynamics));
	elem[0] = current_node->add_child("mks70:mode");
	elem[0]->add_child_text(std::to_string(vcf_env_mode));

	// VCA
	current_node = root_node->add_child("mks70:vca");
	elem[0] = current_node->add_child("mks70:level");
	elem[0]->add_child_text(std::to_string(vca_level));
	elem[0] = current_node->add_child("mks70:mode");
	elem[0]->add_child_text(std::to_string(vca_env_mode));
	elem[0] = current_node->add_child("mks70:dynamics");
	elem[0]->add_child_text(std::to_string(vca_dynamics));

	// LFO
	current_node = root_node->add_child("mks70:lfo");
	elem[0] = current_node->add_child("mks70:waveform");
	elem[0]->add_child_text(std::to_string(lfo_waveform));
	elem[0] = current_node->add_child("mks70:delay");
	elem[0]->add_child_text(std::to_string(lfo_delay_time));
	elem[0] = current_node->add_child("mks70:rate");
	elem[0]->add_child_text(std::to_string(lfo_rate));

	// Envelopes
	for (i = 0; i < 2; i++) {
		xmlnode[i] = root_node->add_child("mks70:envelope");
		xmlnode[i]->set_attribute("index", std::to_string(i + 1));
		elem[i] = xmlnode[i]->add_child("mks70:attack");
		elem[i]->add_child_text(std::to_string(env_attack_time[i]));
		elem[i] = xmlnode[i]->add_child("mks70:decay");
		elem[i]->add_child_text(std::to_string(env_decay_time[i]));
		elem[i] = xmlnode[i]->add_child("mks70:sustain");
		elem[i]->add_child_text(std::to_string(env_sustain_level[i]));
		elem[i] = xmlnode[i]->add_child("mks70:release");
		elem[i]->add_child_text(std::to_string(env_release_time[i]));
		elem[i] = xmlnode[i]->add_child("mks70:key");
		elem[i]->add_child_text(std::to_string(env_key_follow[i]));
	}

	// Chorus
	current_node = root_node->add_child("mks70:chorus");
	elem[0] = current_node->add_child("mks70:mode");
	elem[0]->add_child_text(std::to_string(chorus));

#ifdef HAVE_DEBUG
	doc->write_to_file_formatted(file_name);
#else
	doc->write_to_file(file_name);
#endif

	delete doc;
}

bool mks70_tone::load_from_file(const std::string& file_name)
{
	bool load_ok = true;
	unsigned short index = 0;
	unsigned short value = 0;
	xmlpp::DomParser* parser = 0;
	const xmlpp::Document* doc = 0;
	const xmlpp::Element* root_node = 0;
	xmlpp::Node::NodeList::const_iterator iter;
	xmlpp::Node::NodeList::const_iterator child_iter;
	xmlpp::Element* elem = 0;
	xmlpp::TextNode* text_node = 0;
	xmlpp::Attribute* attribute = 0;
	xmlpp::Element* child_elem = 0;
	Glib::ustring node_name;
	Glib::ustring child_name;
	Glib::ustring text_value;

	try {
		parser = new xmlpp::DomParser(file_name, false);
		doc = parser->get_document();
		if (doc) {
			root_node = doc->get_root_node();
#ifdef HAVE_DEBUG
		std::cout << "Root node namespace: " << root_node->get_namespace_prefix() << std::endl;
		std::cout << "Root node name: " << root_node->get_name() << std::endl;
#endif
			if (root_node->get_name() == "mks70tone") {
				// OK the root node is ok, initialize data
				init_tone();

				const xmlpp::Node::NodeList list = root_node->get_children();
				for (iter = list.begin(); iter != list.end(); ++iter) {
					node_name = (*iter)->get_name();
#ifdef HAVE_DEBUG
		std::cout << "Current: " << node_name << std::endl;
#endif
					if (node_name == "name") {
						elem = dynamic_cast<xmlpp::Element*>(*iter);
						if (elem) {
							text_node = elem->get_child_text();
							if (text_node) {
								text_value = text_node->get_content();
								set_name(text_value);
#ifdef HAVE_DEBUG
	std::cout << "Content: " <<	text_value << std::endl;
#endif
							}
						}
					}
					if (node_name == "dco") {
						index = 0;
						elem = dynamic_cast<xmlpp::Element*>(*iter);
						if (elem) {
							attribute = elem->get_attribute("index");
							if (attribute) {
								text_value = attribute->get_value();
#ifdef HAVE_DEBUG
	std::cout << "Index: " <<	text_value << std::endl;
#endif
								try {
									index = boost::lexical_cast<unsigned short>(text_value);
								}
								catch (const boost::bad_lexical_cast &) {
									index = 0;
								}
							}
							if (index == 1 || index == 2) {
								const xmlpp::Node::NodeList children = elem->get_children();
								for (child_iter = children.begin(); child_iter != children.end(); ++child_iter) {
									child_name = (*child_iter)->get_name();
#ifdef HAVE_DEBUG
		std::cout << "Current: " << child_name << std::endl;
#endif
									child_elem = dynamic_cast<xmlpp::Element*>(*child_iter);
									if (child_elem) {
										text_node = child_elem->get_child_text();
										if (text_node) {
											text_value = text_node->get_content();
#ifdef HAVE_DEBUG
	std::cout << "Value: " << text_value << std::endl;
#endif
											try {
												value = boost::lexical_cast<unsigned short>(text_value);
											}
											catch (const boost::bad_lexical_cast &) {
												value = 0;
											}
										}
									}

									if (child_name == "range")
										set_dco_range (index - 1, value);
									if (child_name == "waveform")
										set_dco_wave(index - 1, value);
									if (child_name == "tune")
										set_dco_tune(index - 1, value);
									if (child_name == "lfo")
										set_dco_lfo(index - 1, value);
									if (child_name == "envelope")
										set_dco_envelope(index - 1, value);
									if (child_name == "xmod" && index == 2)
										set_dco2_crossmod(value);
									if (child_name == "ftune" && index == 2)
										set_dco2_ftune(value);
									if (child_name == "dynamics" && index == 2)
										set_dco_dyna(value);
									if (child_name == "mode" && index == 2)
										set_dco_mode(value);

								}
							}
						}
					} // End dco

					if (node_name == "mixer") {
						index = 0;
						elem = dynamic_cast<xmlpp::Element*>(*iter);
						if (elem) {
							const xmlpp::Node::NodeList children = elem->get_children();
							for (child_iter = children.begin(); child_iter != children.end(); ++child_iter) {
								child_name = (*child_iter)->get_name();
#ifdef HAVE_DEBUG
	std::cout << "Current: " << child_name << std::endl;
#endif
								child_elem = dynamic_cast<xmlpp::Element*>(*child_iter);
								if (child_elem) {
									if (child_name == "dco") {
										attribute = child_elem->get_attribute("index");
										if (attribute) {
											text_value = attribute->get_value();
#ifdef HAVE_DEBUG
	std::cout << "Index: " <<	text_value << std::endl;
#endif
											try {
												index = boost::lexical_cast<unsigned short>(text_value);
											}
											catch (const boost::bad_lexical_cast &) {
												index = 0;
											}
										}
										if (index == 1 || index == 2) {
											text_node = child_elem->get_child_text();
											if (text_node) {
												text_value = text_node->get_content();
#ifdef HAVE_DEBUG
	std::cout << "Value: " << text_value << std::endl;
#endif
												try {
													value = boost::lexical_cast<unsigned short>(text_value);
												}
												catch (const boost::bad_lexical_cast &) {
													value = 0;
												}
											}
											set_mixer_dco(index -1, value);
										}
									}

									text_node = child_elem->get_child_text();
									if (text_node) {
										text_value = text_node->get_content();
#ifdef HAVE_DEBUG
std::cout << "Value: " << text_value << std::endl;
#endif
										try {
											value = boost::lexical_cast<unsigned short>(text_value);
										}
										catch (const boost::bad_lexical_cast &) {
											value = 0;
										}
									}

									if (child_name == "envelope")
										set_mixer_envelope(value);
									if (child_name == "dynamics")
										set_mixer_dyna(value);
									if (child_name == "mode")
										set_mixer_mode(value);
								}
							}
						}
					} // End mixer

				}
			}
			else load_ok = false;
		}
		else load_ok = false;
	}
	catch (const xmlpp::parse_error& error) {
#ifdef HAVE_DEBUG
		std::cerr << "Parse error: " << error.what() << std::endl;
#endif
		load_ok = false;
	}

	delete parser; parser = 0;
	return load_ok;
}

// Get methods
unsigned short mks70_tone::get_dco_range(unsigned short dco)
{
	if (dco < 2) return dco_range[dco];
	return 0;
}
unsigned short mks70_tone::get_dco_waveform(unsigned short dco)
{
	if (dco < 2) return dco_wave[dco];
	return 0;
}
unsigned short mks70_tone::get_dco_tune(unsigned short dco)
{
	if (dco < 2) return dco_tune[dco];
	return 0;
}
unsigned short mks70_tone::get_dco_lfo(unsigned short dco)
{
	if (dco < 2) return dco_lfo[dco];
	return 0;
}
unsigned short mks70_tone::get_dco_envelope(unsigned short dco)
{
	if (dco < 2) return dco_env[dco];
	return 0;
}
unsigned short mks70_tone::get_mix_dco(unsigned short dco)
{
	if (dco < 2) return mix_dco[dco];
	return 0;
}
unsigned short mks70_tone::get_envelope_attack_time(unsigned short envelope)
{
	if (envelope < 2) return env_attack_time[envelope];
	return 0;
}
unsigned short mks70_tone::get_envelope_decay_time(unsigned short envelope)
{
	if (envelope < 2) return env_decay_time[envelope];
	return 0;
}
unsigned short mks70_tone::get_envelope_sustain_level(unsigned short envelope)
{
	if (envelope < 2) return env_sustain_level[envelope];
	return 0;
}
unsigned short mks70_tone::get_envelope_release_time(unsigned short envelope)
{
	if (envelope < 2) return env_release_time[envelope];
	return 0;
}
unsigned short mks70_tone::get_envelope_key_follow(unsigned short envelope)
{
	if (envelope < 2) return env_key_follow[envelope];
	return 0;
}

#ifdef HAVE_DEBUG
void mks70_tone::dump_tone() const
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
		std::cout << std::setbase(10) << "Envelope " << (i + 1) << " Attack: "
			<< env_attack_time[i] << std::endl;
		std::cout << std::setbase(10) << "Envelope " << (i + 1) << " Decay: "
			<< env_decay_time[i] << std::endl;
		std::cout << std::setbase(10) << "Envelope " << (i + 1) << " Sustain: "
			<< env_sustain_level[i] << std::endl;
		std::cout << std::setbase(10) << "Envelope " << (i + 1) << " Release: "
			<< env_release_time[i] << std::endl;
		std::cout << std::setbase(10) << "Envelope " << (i + 1) << " Key Follow: "
			<< env_key_follow[i] << std::endl;
	}
	std::cout << std::setbase(10) << "DCO2 XMod: " << dco2_xmod << std::endl;
	std::cout << std::setbase(10) << "DCO2 FTune: " << dco2_ftune << std::endl;
	std::cout << std::setbase(10) << "DCO Dyna: " << dco_dynamics << std::endl;
	std::cout << std::setbase(10) << "DCO Mode: " << dco_mode << std::endl;
	std::cout << std::setbase(10) << "Mixer envelope: " << mix_env << std::endl;
	std::cout << std::setbase(10) << "Mixer Dyna: " << mix_dynamics << std::endl;
	std::cout << std::setbase(10) << "Mixer Mode: " << mix_env_mode << std::endl;
	std::cout << std::setbase(10) << "VCF HPF: " << vcf_hpf << std::endl;
	std::cout << std::setbase(10) << "VCF Cutoff: " << vcf_cutoff_freq << std::endl;
	std::cout << std::setbase(10) << "VCF Resonance: " << vcf_resonance << std::endl;
	std::cout << std::setbase(10) << "VCF LFO: " << vcf_lfo_mod_depth << std::endl;
	std::cout << std::setbase(10) << "VCF Envelope: " << vcf_env_mod_depth << std::endl;
	std::cout << std::setbase(10) << "VCF Key Follow: " << vcf_key_follow << std::endl;
	std::cout << std::setbase(10) << "VCA Level: " << vca_level << std::endl;
	std::cout << std::setbase(10) << "VCA Dynamics: " << vca_dynamics << std::endl;
	std::cout << std::setbase(10) << "VCA Envelope Mode: " << vca_env_mode << std::endl;
	std::cout << std::setbase(10) << "LFO Waveform: " << lfo_waveform << std::endl;
	std::cout << std::setbase(10) << "LFO Delay Time: " << lfo_delay_time << std::endl;
	std::cout << std::setbase(10) << "LFO Rate: " << lfo_rate << std::endl;
	std::cout << std::setbase(10) << "Chorus: " << chorus << std::endl;
}
#endif

const std::string mks70_tone::init_tone_name = "INIT TONE";