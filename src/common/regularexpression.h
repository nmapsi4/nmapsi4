/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
 *   francesco.cecconi@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef REGULAREXPRESSION_H
#define REGULAREXPRESSION_H

static const char matchPorts[] = "^\\d{1,5}/+\\d?";

static const char matchTraceroute[] = "^\\d\\d?";

static const char matchIpv4[] = "((([2][5][0-5]|([2][0-4]|[1][0-9]|[0-9])?[0-9])\\.){3})"
    "([2][5][0-5]|([2][0-4]|[1][0-9]|[0-9])?[0-9])";

static const char matchDNS[] = "^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)"
    "*([A-Za-z]|[A-Za-z][A-Za-z0-9\\-]*[A-Za-z0-9])$";

#endif // REGULAREXPRESSION_H
