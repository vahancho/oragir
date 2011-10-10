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
    // Http status messages
    const char *sHostLookup = QT_TRANSLATE_NOOP("GHerald", "A host name lookup is in progress...");
    const char *sConnecting = QT_TRANSLATE_NOOP("GHerald", "Connecting to the host...");
    const char *sSending = QT_TRANSLATE_NOOP("GHerald", "Sending request to the server...");
    const char *sReading = QT_TRANSLATE_NOOP("GHerald", "Reading the server's response...");
    const char *sConnected = QT_TRANSLATE_NOOP("GHerald", "Connected.");

    // Other strings
    const char *sOk = QT_TRANSLATE_NOOP("GHerald", "OK");
    const char *sCancel = QT_TRANSLATE_NOOP("GHerald", "Cancel");

} // namespace str
