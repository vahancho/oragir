/**************************************************************************
*   Copyright (C) 2011 by Vahan Aghajanyan                                *
*   vahancho@gmail.com                                                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
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

#ifndef __GUISTRINGS_H__
#define __GUISTRINGS_H__

#include <QApplication>

#define TRANSLATE(string) qApp->translate("GHerald", string)

namespace str
{
    //////////////////////////////////////////////////////////////////////////
    // Translatable GUI Strings
    //   

    // Http status messages
    extern const char *sHostLookup;
    extern const char *sConnecting;
    extern const char *sSending;
    extern const char *sReading;
    extern const char *sConnected;

    // Other strings
    extern const char *sOk;
    extern const char *sCancel;

} // namespace str

#endif // __GUISTRINGS_H__
