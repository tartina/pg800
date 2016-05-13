/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * bulkdump_exception.h
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

#ifndef _BULKDUMP_EXCEPTION_H_
#define _BULKDUMP_EXCEPTION_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <exception>

class mks70_bulkdump_exception : public std::exception
{
public:
	mks70_bulkdump_exception(const std::string& message) {
		this->message = message;
	}

	virtual std::string& what() {
		return message;
	}

private:
	std::string message;
};

#endif // _BULKDUMP_EXCEPTION_H_
