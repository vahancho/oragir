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
    extern const char *MenuStream;
    extern const char *MenuTools;

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
    extern const char *ActionNewFolder;
    extern const char *ActionStart;
    extern const char *ActionStop;
    extern const char *ActionFilters;
    extern const char *ActionFilterExport;
    extern const char *ActionFilterImport;
    extern const char *ActionOptions;
    extern const char *ActionDelete;
    extern const char *ActionHelp;

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
    extern const char *QuitOnCloseTitle;
    extern const char *GeneralPageTitle;
    extern const char *ConnectionPageTitle;
    extern const char *AdvancedPageTitle;
    extern const char *OptionsDialogTitle;
    extern const char *CheckUpdatesTitle;
    extern const char *ReconnectTitle;
    extern const char *FoldersTitle;
    extern const char *ProcessedRecords;
    extern const char *Downloaded;
    extern const char *Unread;
    extern const char *Total;
    extern const char *ExportFilterTitle;
    extern const char *ImportFilterTitle;
    extern const char *FilterDialogFilter;

    // Error messages.
    extern const char *DatabaseError;

} // namespace str

#endif // __GUISTRINGS_H__
