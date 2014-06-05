/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * tone.h
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

#ifndef MKS70_TONE_H
#define MKS70_TONE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include "RtMidi.h"

class mks70_tone
{
private:
	std::string name;
	unsigned short tone_number = 0; // Tone A = 0, tone B = 1

	unsigned short dco_range[2]; // 4 values
	unsigned short dco_wave[2]; // 4 values
	unsigned short dco_tune[2];
	unsigned short dco_lfo[2];
	unsigned short dco_env[2];
	unsigned short dco2_xmod; // 4 values
	unsigned short dco2_ftune;
	unsigned short dco_dynamics; // 4 values
	unsigned short dco_mode; // 4 values

	unsigned short mix_dco[2];
	unsigned short mix_env;
	unsigned short mix_dynamics; // 4 values
	unsigned short mix_env_mode; // 4 values

	unsigned short vcf_hpf; // 4 values
	unsigned short vcf_cutoff_freq;
	unsigned short vcf_resonance;
	unsigned short vcf_lfo_mod_depth;
	unsigned short vcf_env_mod_depth;
	unsigned short vcf_key_follow;
	unsigned short vcf_dynamics; // 4 values
	unsigned short vcf_env_mode; // 4 values

	unsigned short vca_level;
	unsigned short vca_dynamics; // 4 values
	unsigned short vca_env_mode; // 2 values

	unsigned short chorus; // 3 values

	unsigned short lfo_waveform; // 3 values
	unsigned short lfo_delay_time;
	unsigned short lfo_rate;

	unsigned short env_attack_time[2];
	unsigned short env_decay_time[2];
	unsigned short env_sustain_level[2];
	unsigned short env_release_time[2];
	unsigned short env_key_follow[2]; // 4 values

	std::vector<unsigned char> message;

	void init_tone();

public:
	static const std::string init_tone_name;
	mks70_tone();
	virtual ~mks70_tone();
	mks70_tone& operator=(const mks70_tone& other);

	void apr_send(unsigned short midi_channel, RtMidiOut* midi_out);

	inline std::string get_name() {return name;};

	void set_dco_range(unsigned short dco, unsigned short range,
	                   unsigned short midi_channel, RtMidiOut* midi_out, bool send = false);
	void set_dco_wave(unsigned short dco, unsigned short wave,
	                  unsigned short midi_channel, RtMidiOut* midi_out, bool send = false);
	void set_dco2_crossmod(unsigned short crossmod, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_dco_tune(unsigned short dco, unsigned short tune,
	                  unsigned short midi_channel, RtMidiOut* midi_out, bool send = false);
	void set_dco2_ftune(unsigned short ftune, unsigned short midi_channel,
	                    RtMidiOut* midi_out, bool send = false);
	void set_dco_lfo(unsigned short dco, unsigned short lfo,
	                  unsigned short midi_channel, RtMidiOut* midi_out, bool send = false);
	void set_dco_envelope(unsigned short dco, unsigned short envelope,
	                  unsigned short midi_channel, RtMidiOut* midi_out, bool send = false);
	void set_dco_dyna(unsigned short dyna, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_dco_mode(unsigned short mode, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_mixer_dco(unsigned short dco, unsigned short value,
	                  unsigned short midi_channel, RtMidiOut* midi_out, bool send = false);
	void set_mixer_envelope(unsigned short envelope, unsigned short midi_channel,
	                        RtMidiOut* midi_out, bool send = false);
	void set_mixer_dyna(unsigned short dyna, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_mixer_mode(unsigned short mode, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_vcf_hpf(unsigned short value, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_vcf_cutoff(unsigned short value, unsigned short midi_channel,
	                        RtMidiOut* midi_out, bool send = false);
	void set_vcf_resonance(unsigned short value, unsigned short midi_channel,
	                        RtMidiOut* midi_out, bool send = false);
	void set_vcf_lfo(unsigned short value, unsigned short midi_channel,
	                        RtMidiOut* midi_out, bool send = false);
	void set_vcf_envelope(unsigned short value, unsigned short midi_channel,
	                        RtMidiOut* midi_out, bool send = false);
	void set_vcf_key(unsigned short value, unsigned short midi_channel,
	                        RtMidiOut* midi_out, bool send = false);
	void set_vcf_dyna(unsigned short dyna, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_vcf_env_mode(unsigned short mode, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_vca_level(unsigned short value, unsigned short midi_channel,
	                        RtMidiOut* midi_out, bool send = false);
	void set_vca_dyna(unsigned short dyna, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_vca_env_mode(unsigned short mode, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_lfo_waveform(unsigned short waveform, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_lfo_delay_time(unsigned short value, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_lfo_rate(unsigned short value, unsigned short midi_channel,
	                       RtMidiOut* midi_out, bool send = false);
	void set_envelope_attack(unsigned short envelope, unsigned short value,
	                         unsigned short midi_channel, RtMidiOut* midi_out,
	                         bool send = false);
	void set_envelope_decay(unsigned short envelope, unsigned short value,
	                         unsigned short midi_channel, RtMidiOut* midi_out,
	                         bool send = false);
	void set_envelope_sustain(unsigned short envelope, unsigned short value,
	                         unsigned short midi_channel, RtMidiOut* midi_out,
	                         bool send = false);
	void set_envelope_release(unsigned short envelope, unsigned short value,
	                         unsigned short midi_channel, RtMidiOut* midi_out,
	                         bool send = false);
	void set_envelope_key_follow(unsigned short envelope, unsigned short value,
	                         unsigned short midi_channel, RtMidiOut* midi_out,
	                         bool send = false);
	void set_chorus(unsigned short value, unsigned short midi_channel,
	                RtMidiOut* midi_out, bool send = false);

	void set_name(const std::string& name);
	void set_tone_number(const unsigned short tone);
	unsigned short get_tone_number() {return tone_number;};

	void save_to_file(const std::string& file_name) const;
	bool load_from_file(const std::string& file_name);

#ifdef HAVE_DEBUG
	void dump_tone() const;
#endif
};

#endif
