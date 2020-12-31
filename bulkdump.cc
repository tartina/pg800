/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * bulkdump.cc
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_DEBUG
#include <iomanip>
#else
#define NDEBUG
#endif

#include <iostream>
#include <fstream>
#include <string>

#ifdef HAVE_BOOST_ALGORITHM_STRING_HPP
#include <boost/algorithm/string.hpp>
#else
#error We need boost string trim function
#endif

#include "bulkdump.h"
#include "bulkdump_exception.h"

#define BULKDUMP_SIZE 10234
#define TONE_DUMP_START 6784
#define TONE_DUMP_SIZE 69

mks70_bulkdump::mks70_bulkdump(const std::string& file_name)
{
	unsigned short i;
	unsigned tone_index;

	dump = new char[BULKDUMP_SIZE];

	std::ifstream file(file_name, std::ios::in|std::ios::binary);

	if (file.is_open()) {
		file.read(dump, BULKDUMP_SIZE);
		if (file.fail()) {
			throw mks70_bulkdump_exception("Error loading file");
		}
		file.close();
	}

	// Do some checks on dump
	if (dump[0] != '\xf0' || dump[1] != '\x41' || dump[2] != '\x37')
		throw mks70_bulkdump_exception("Wrong file header format");
	if (dump[BULKDUMP_SIZE - 1] != '\xf7')
		throw mks70_bulkdump_exception("Wrong file end format");

	/* Search for tone names: */
	for (i = 0; i < TONE_NUMBER; ++i) {
		tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * i;

#ifdef HAVE_DEBUG
		std::cout << "i: " << i << ", tone index: " << tone_index << std::endl;
		std::cout << "Tone dump" << std::hex << std::endl;
		for (unsigned short k = 0; k < TONE_DUMP_SIZE; ++k)
			std::cout << k << ": " << (+dump[k + tone_index] & 0xFF) << std::endl;
#endif
		// Check
		if (dump[tone_index] != '\xf0' || dump[tone_index + 1] != '\x41' || dump[tone_index + 2] != '\x37' ||
				dump[tone_index + 4] != '\x24' || dump[tone_index + 5] != '\x20')
			throw mks70_bulkdump_exception("Wrong tone header format");
		if (dump[tone_index + TONE_DUMP_SIZE - 1] != '\xf7')
			throw mks70_bulkdump_exception("Wrong tone end format");

		// Construct tone name
		tone_name[i] = new std::string(&dump[tone_index + 9], 10);
		boost::algorithm::trim(*tone_name[i]);
	}
}

mks70_bulkdump::~mks70_bulkdump()
{
	for (unsigned short i = 0; i < TONE_NUMBER; ++i) delete tone_name[i];
	delete[] dump;
}

const std::string& mks70_bulkdump::get_tone_name(unsigned short index) const
{
	if (index < TONE_NUMBER) return *tone_name[index];
	return INVALID_TONE_NAME;
}

const unsigned short mks70_bulkdump::get_dco_range(unsigned short tone, unsigned short dco) const
{
	if (tone < TONE_NUMBER && dco < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 11 + 5 * dco] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_dco_waveform(unsigned short tone, unsigned short dco) const
{
	if (tone < TONE_NUMBER && dco < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 12 + 5 * dco] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_dco2_crossmod(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 18] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_dco_tune(unsigned short tone, unsigned short dco) const
{
	if (tone < TONE_NUMBER && dco < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 13 + 6 * dco];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_dco2_fine_tune(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 20];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_dco_lfo_mod_depth(unsigned short tone, unsigned short dco) const
{
	if (tone < TONE_NUMBER && dco < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 14 + 7 * dco];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_dco_lfo_env_depth(unsigned short tone, unsigned short dco) const
{
	if (tone < TONE_NUMBER && dco < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 15 + 7 * dco];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_dco_dynamics(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 26] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_dco_env_mode(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 27] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_mixer_dco(unsigned short tone, unsigned short dco) const
{
	if (tone < TONE_NUMBER && dco < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 28 + dco];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_mixer_env_mode_depth(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 30];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_mixer_dynamics(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 31] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_mixer_env_mode(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 32] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_hpf_cutoff_freq(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 33] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vcf_cutoff_freq(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 34];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vcf_resonance(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 35];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vcf_lfo_mod_depth(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 36];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vcf_env_mod_depth(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 37];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vcf_key_follow(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 38];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vcf_dynamics(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 39] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vcf_env_mode(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 40] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vca_level(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 41];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vca_dynamics(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 42] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_chorus(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		// Only 3 values
		auto value = dump[tone_index + 9 + 43] / 32;
		if (value == 3) value = 2;
		return value;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_lfo_waveform(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		// Only 3 values
		auto value = dump[tone_index + 9 + 44] / 32;
		if (value == 3) value = 2;
		return value;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_lfo_delay_time(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 45];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_lfo_rate(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 46];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_env_attack_time(unsigned short tone, unsigned short env) const
{
	if (tone < TONE_NUMBER && env < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 47 + env * 5];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_env_decay_time(unsigned short tone, unsigned short env) const
{
	if (tone < TONE_NUMBER && env < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 48 + env * 5];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_env_sustain_level(unsigned short tone, unsigned short env) const
{
	if (tone < TONE_NUMBER && env < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 49 + env * 5];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_env_release_time(unsigned short tone, unsigned short env) const
{
	if (tone < TONE_NUMBER && env < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 50 + env * 5];
	} else return 0;
}
const unsigned short mks70_bulkdump::get_env_key_follow(unsigned short tone, unsigned short env) const
{
	if (tone < TONE_NUMBER && env < 2) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 51 + env * 5] / 32;
	} else return 0;
}
const unsigned short mks70_bulkdump::get_vca_env_mode(unsigned short tone) const
{
	if (tone < TONE_NUMBER) {
		auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
		return dump[tone_index + 9 + 58] / 64;
	} else return 0;
}

const unsigned short mks70_bulkdump::TONE_NUMBER = 50;
const std::string mks70_bulkdump::INVALID_TONE_NAME = "[INVALID NAME]";

#ifdef HAVE_DEBUG
void mks70_bulkdump::dump_tone(unsigned short tone) const
{
	std::cout << "Tone dump: " << tone << std::endl;
	auto tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * tone;
	for (unsigned short k = 0; k < TONE_DUMP_SIZE; ++k)
		std::cout << k - 9 << ": " << (+dump[k + tone_index] & 0xFF) << std::endl;
}
#endif
