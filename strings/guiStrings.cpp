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

#include <QtGlobal>


namespace str
{
//////////////////////////////////////////////////////////////////////////
// Translatable GUI Strings
//
    // Menu names.
    const char *MenuFile = QT_TRANSLATE_NOOP("Oragir", "&File");
    const char *MenuNew = QT_TRANSLATE_NOOP("Oragir", "New");
    const char *MenuView = QT_TRANSLATE_NOOP("Oragir", "&View");
    const char *MenuViews = QT_TRANSLATE_NOOP("Oragir", "&Views");
    const char *MenuToolBars = QT_TRANSLATE_NOOP("Oragir", "&Toolbars");
    const char *MenuWindow = QT_TRANSLATE_NOOP("Oragir", "&Window");
    const char *MenuHelp = QT_TRANSLATE_NOOP("Oragir", "&Help");
    const char *MenuStream = QT_TRANSLATE_NOOP("Oragir", "&Stream");
    const char *MenuTools = QT_TRANSLATE_NOOP("Oragir", "&Tools");

    const char *ActionMinimize = QT_TRANSLATE_NOOP("Oragir", "Mi&nimize");
    const char *ActionMaximize = QT_TRANSLATE_NOOP("Oragir", "Ma&ximize");
    const char *ActionRestore = QT_TRANSLATE_NOOP("Oragir", "&Restore");
    const char *ActionQuit = QT_TRANSLATE_NOOP("Oragir", "&Quit");
    const char *ActionExit = QT_TRANSLATE_NOOP("Oragir", "&Exit");
    const char *ActionStatusBar = QT_TRANSLATE_NOOP("Oragir", "Status &Bar");
    const char *ActionClose = QT_TRANSLATE_NOOP("Oragir", "Cl&ose");
    const char *ActionCloseAll = QT_TRANSLATE_NOOP("Oragir", "Close &All");
    const char *ActionTile = QT_TRANSLATE_NOOP("Oragir", "&Tile");
    const char *ActionCascade = QT_TRANSLATE_NOOP("Oragir", "&Cascade");
    const char *ActionNext = QT_TRANSLATE_NOOP("Oragir", "&Next");
    const char *ActionPrevious = QT_TRANSLATE_NOOP("Oragir", "&Previous");
    const char *ActionAbout = QT_TRANSLATE_NOOP("Oragir", "&About");
    const char *ActionOpen = QT_TRANSLATE_NOOP("Oragir", "&Open Database");
    const char *ActionStart = QT_TRANSLATE_NOOP("Oragir", "&Start");
    const char *ActionStop = QT_TRANSLATE_NOOP("Oragir", "S&top");
    const char *ActionFilters = QT_TRANSLATE_NOOP("Oragir", "&Filters...");
    const char *ActionOptions = QT_TRANSLATE_NOOP("Oragir", "&Options...");

    // Http status messages
    const char *HostLookup = QT_TRANSLATE_NOOP("Oragir", "A host name lookup is in progress...");
    const char *Connecting = QT_TRANSLATE_NOOP("Oragir", "Connecting to the host...");
    const char *Sending = QT_TRANSLATE_NOOP("Oragir", "Sending request to the server...");
    const char *Reading = QT_TRANSLATE_NOOP("Oragir", "Reading the server's response...");
    const char *Connected = QT_TRANSLATE_NOOP("Oragir", "Connected.");

    // Other strings
    const char *Ok = QT_TRANSLATE_NOOP("Oragir", "OK");
    const char *Cancel = QT_TRANSLATE_NOOP("Oragir", "Cancel");
    const char *AboutTitle = QT_TRANSLATE_NOOP("Oragir", "About Oragir");
    const char *About = QT_TRANSLATE_NOOP("Oragir",
                                          "<big><b>Oragir v%1</b></big>"
                                          "<BR>"
                                          "<BR>"
                                          "Copyright © 2011 "
                                          "<a href=\"mailto:vahancho@gmail.com\">Vahan Aghajanyan</a>."
                                          "<BR>"
                                          "All Rights Reserved."
                                          "<BR><BR>"
                                          "Oragir is made possible by the "
                                          "<a href=\"http://sourceforge.net/projects/oragir\">Oragir</a> "
                                          "open source project.");

    // Error messages.
    const char *DatabaseError = QT_TRANSLATE_NOOP("Oragir", "Database Error");

} // namespace str
