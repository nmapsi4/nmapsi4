/*
Copyright 2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGULAREXPRESSION_H
#define REGULAREXPRESSION_H

static const char matchPorts[] = "^\\d{1,5}/.{1,5}\\s+\\b(open|closed|filtered|unfiltered)\\b";

static const char matchTraceroute[] = "^\\d\\s+\\d+\\.\\d+\\s+\\bms\\b\\s+";

static const char matchIpv4[] = "((([2][5][0-5]|([2][0-4]|[1][0-9]|[0-9])?[0-9])\\.){3})"
                                "([2][5][0-5]|([2][0-4]|[1][0-9]|[0-9])?[0-9])";

static const char matchDNS[] = "^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)"
                               "*([A-Za-z]|[A-Za-z][A-Za-z0-9\\-]*[A-Za-z0-9])$";

#endif // REGULAREXPRESSION_H
