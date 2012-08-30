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

#ifndef NOTIFY_H
#define NOTIFY_H

#include "style.h"
#include "pushbuttonorientated.h"
#include <config.h>

// qt includes
#include <QtCore/QDebug>

// kde includes
#ifdef USE_KDELIBS
#include <knotification.h>
#endif

class Notify
{

public:
    static void startButtonNotify(PushButtonOrientated* button);
    static void setCheckedNotify(PushButtonOrientated* button);
    static void clearButtonNotify(PushButtonOrientated* button);

};

#endif // NOTIFY_H
