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
	vca_level = 0;
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
	xmlpp::Element* dco[2] = {0,};
	xmlpp::Element* elem[2] = {0,};

	doc = new xmlpp::Document();
	root_node = doc->create_root_node("mks70tone", "http://heavyware.it/namespace/mks70",
	                                  "mks70");
	current_node = root_node->add_child("mks70:name");
	current_node->add_child_text(name);

	for (i = 0; i < 2; i++) {
		dco[i] = root_node->add_child("mks70:dco");
		dco[i]->set_attribute("index", std::to_string(i + 1));
		elem[i] = dco[i]->add_child("mks70:range");
		elem[i]->add_child_text(std::to_string(dco_range[i]));
		elem[i] = dco[i]->add_child("mks70:waveform");
		elem[i]->add_child_text(std::to_string(dco_wave[i]));
		elem[i] = dco[i]->add_child("mks70:tune");
		elem[i]->add_child_text(std::to_string(dco_tune[i]));
		elem[i] = dco[i]->add_child("mks70:lfo");
		elem[i]->add_child_text(std::to_string(dco_lfo[i]));
		elem[i] = dco[i]->add_child("mks70:envelope");
		elem[i]->add_child_text(std::to_string(dco_env[i]));
	}
	elem[1] = dco[1]->add_child("mks70:xmod");
	elem[1]->add_child_text(std::to_string(dco2_xmod));
	elem[1] = dco[1]->add_child("mks70:ftune");
	elem[1]->add_child_text(std::to_string(dco2_ftune));
	elem[1] = dco[1]->add_child("mks70:dynamics");
	elem[1]->add_child_text(std::to_string(dco_dynamics));
	elem[1] = dco[1]->add_child("mks70:mode");
	elem[1]->add_child_text(std::to_string(dco_mode));

	current_node = root_node->add_child("mks70:mixer");
	for (i = 0; i < 2; i++) {
		dco[i] = current_node->add_child("mks70:dco");
		dco[i]->set_attribute("index", std::to_string(i + 1));
		dco[i]->add_child_text(std::to_string(mix_dco[i]));
	}
	
#ifdef HAVE_DEBUG
	doc->write_to_file_formatted(file_name);
#else
	doc->write_to_file(file_name);
#endif

	delete doc;
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