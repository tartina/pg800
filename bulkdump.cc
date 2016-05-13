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

#include "bulkdump.h"
#include "bulkdump_exception.h"

#define BULKDUMP_SIZE 10234
#define TONE_DUMP_START 6784
#define TONE_DUMP_SIZE 69
#define TONE_NUMBER 50

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

#ifdef HAVE_DEBUG
	std::cout << "Bulk dump: " << std::hex;
	for (i = 0; i < BULKDUMP_SIZE; ++i) std::cout << (+dump[i] & 0xFF) << " ";
	std::cout << std::endl;
#endif

	// Do some checks on dump
	if (dump[0] != '\xf0' || dump[1] != '\x41' || dump[2] != '\x37')
		throw mks70_bulkdump_exception("Wrong file header  format");
	if (dump[BULKDUMP_SIZE - 1] != '\xf7')
		throw mks70_bulkdump_exception("Wrong file end format");

	/* Search for tone names: */
	for (i = 0; i < TONE_NUMBER; ++i) {
		tone_index = TONE_DUMP_START + TONE_DUMP_SIZE * i;

#ifdef HAVE_DEBUG
		std::cout << "i: " << i << ", tone index: " << tone_index << std::endl;
		std::cout << "Tone dump: " << std::hex;
		for (unsigned short k = tone_index; k < tone_index + TONE_DUMP_SIZE; ++k) std::cout << (+dump[k] & 0xFF) << " ";
		std::cout << std::endl;
#endif
		// Check
		if (dump[tone_index] != '\xf0' || dump[tone_index + 1] != '\x41' || dump[tone_index + 2] != '\x37' ||
				dump[tone_index + 4] != '\x24' || dump[tone_index + 5] != '\x20')
			throw mks70_bulkdump_exception("Wrong tone header format");
		if (dump[tone_index + TONE_DUMP_SIZE - 1] != '\xf7')
			throw mks70_bulkdump_exception("Wrong tone end format");

		// Construct tone name
		tone_name[i] = new std::string(&dump[tone_index + 9], 10);
#ifdef HAVE_DEBUG
		std::cout << "Tone " << i << " name: " << *tone_name[i] << std::endl;
#endif
	}
}

mks70_bulkdump::~mks70_bulkdump()
{
	for (unsigned short i = 0; i < TONE_NUMBER; ++i) delete tone_name[i];
	delete[] dump;
}

const std::string& mks70_bulkdump::get_tone_name(unsigned short index) const
{
	if (index < 50) return *tone_name[index];
	return INVALID_TONE_NAME;
}

const std::string mks70_bulkdump::INVALID_TONE_NAME = "[INVALID NAME]";
