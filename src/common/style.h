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

#ifndef STYLE_H
#define STYLE_H

static const char verticalStyleSheet[] = "QToolButton:checked {border: 2px; border-radius: 2px; "
        "background-color: palette(highlight); color: palette(highlighted-text); "
        "margin-left: 0px; margin-right: 3px; padding: 4px;} ";

static const char nofifyStyleSheet[] = "QToolButton { background-color: palette(highlight) } ";

// sidebar button width
const int verticalButtonWidth = 25;

static const char positiveBackground[] = "background: #C0FFC0;";
static const char negativeBackground[] = "background: #FFC0C0;";
static const char highIpNumberBackground[] = "background: #FFDCA8;";
static const char neutralBackground[] = "background: #fff;";

#endif
