/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * midi.cc
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

#include "midi.h"

midi::midi()
{
	try {
		midiout = new RtMidiOut();
	}
	catch ( const RtError &error ) {
		// TODO handle error
#ifdef HAVE_DEBUG
		error.printMessage();
#endif
	}
	number_of_ports = midiout->getPortCount();
#ifdef HAVE_DEBUG
		std::cout << "Number of MIDI ports: " << number_of_ports << "\n";
#endif
}

midi::~midi()
{
	delete midiout;
}