/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * bulkdump.h
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

#ifndef _BULKDUMP_H_
#define _BULKDUMP_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <fstream>
#include <string>

class mks70_bulkdump
{
public:
	mks70_bulkdump(const std::string& file_name);
	virtual ~mks70_bulkdump();
	mks70_bulkdump(const mks70_bulkdump& other) = delete;
	mks70_bulkdump& operator=(const mks70_bulkdump& other) = delete;

	// Getter functions
	const std::string& get_tone_name(unsigned short index) const;
	const unsigned short get_dco_range(unsigned short tone, unsigned short dco) const;
	const unsigned short get_dco_waveform(unsigned short tone, unsigned short dco) const;
	const unsigned short get_dco2_crossmod(unsigned short tone) const;
	const unsigned short get_dco_tune(unsigned short tone, unsigned short dco) const;
	const unsigned short get_dco2_fine_tune(unsigned short tone) const;
	const unsigned short get_dco_lfo_mod_depth(unsigned short tone, unsigned short dco) const;
	const unsigned short get_dco_lfo_env_depth(unsigned short tone, unsigned short dco) const;
	const unsigned short get_dco_dynamics(unsigned short tone) const;
	const unsigned short get_dco_env_mode(unsigned short tone) const;
	const unsigned short get_mixer_dco(unsigned short tone, unsigned short dco) const;
	const unsigned short get_mixer_env_mode_depth(unsigned short tone) const;
	const unsigned short get_mixer_dynamics(unsigned short tone) const;
	const unsigned short get_mixer_env_mode(unsigned short tone) const;
	const unsigned short get_hpf_cutoff_freq(unsigned short tone) const;
	const unsigned short get_vcf_cutoff_freq(unsigned short tone) const;
	const unsigned short get_vcf_resonance(unsigned short tone) const;
	const unsigned short get_vcf_lfo_mod_depth(unsigned short tone) const;
	const unsigned short get_vcf_env_mod_depth(unsigned short tone) const;
	const unsigned short get_vcf_key_follow(unsigned short tone) const;
	const unsigned short get_vcf_dynamics(unsigned short tone) const;
	const unsigned short get_vcf_env_mode(unsigned short tone) const;
	const unsigned short get_vca_level(unsigned short tone) const;
	const unsigned short get_vca_dynamics(unsigned short tone) const;
	const unsigned short get_chorus(unsigned short tone) const;
	const unsigned short get_lfo_waveform(unsigned short tone) const;
	const unsigned short get_lfo_delay_time(unsigned short tone) const;
	const unsigned short get_lfo_rate(unsigned short tone) const;
	const unsigned short get_env_attack_time(unsigned short tone, unsigned short env) const;
	const unsigned short get_env_decay_time(unsigned short tone, unsigned short env) const;
	const unsigned short get_env_sustain_level(unsigned short tone, unsigned short env) const;
	const unsigned short get_env_release_time(unsigned short tone, unsigned short env) const;
	const unsigned short get_env_key_follow(unsigned short tone, unsigned short env) const;
	const unsigned short get_vca_env_mode(unsigned short tone) const;

#ifdef HAVE_DEBUG
	void dump_tone(unsigned short tone) const;
#endif

	static const unsigned short TONE_NUMBER;
	static const std::string INVALID_TONE_NAME;

private:
	char *dump; // The dump buffer 10234 bytes
	std::string *tone_name[50]; // A dump contains 50 tones
};

#endif // _BULKDUMP_H_
