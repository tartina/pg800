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
	const std::string& get_tone_name(unsigned short index) const;

	static const std::string INVALID_TONE_NAME;

protected:

private:
	char *dump; // The dump buffer 10234 bytes
	std::string *tone_name[50]; // A dump contains 50 tones
};

#endif // _BULKDUMP_H_
