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

    // Menu names.
    extern const char *MenuFile;
    extern const char *MenuNew;
    extern const char *MenuView;
    extern const char *MenuViews;
    extern const char *MenuToolBars;
    extern const char *MenuWindow;
    extern const char *MenuHelp;

    // Action titles.
    extern const char *ActionMinimize;
    extern const char *ActionMaximize;
    extern const char *ActionRestore;
    extern const char *ActionQuit;
    extern const char *ActionExit;
    extern const char *ActionStatusBar;
    extern const char *ActionClose;
    extern const char *ActionCloseAll;
    extern const char *ActionTile;
    extern const char *ActionCascade;
    extern const char *ActionNext;
    extern const char *ActionPrevious;
    extern const char *ActionAbout;

    // Http status messages
    extern const char *HostLookup;
    extern const char *Connecting;
    extern const char *Sending;
    extern const char *Reading;
    extern const char *Connected;

    // Other strings
    extern const char *Ok;
    extern const char *Cancel;
    extern const char *AboutTitle;
    extern const char *About;

} // namespace str

#endif // __GUISTRINGS_H__
