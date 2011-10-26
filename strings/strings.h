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

#ifndef __STRINGS_H__
#define __STRINGS_H__

namespace str
{
    //////////////////////////////////////////////////////////////////////////
    // Not Translatable Strings
    //

    // Application name
    extern const char *sAppName;

    extern const char *sFeedUrl;

    // XML Tags
    extern const char *sTagFeed;
    extern const char *sTagTitle;
    extern const char *sTagName;
    extern const char *sTagJournal;
    extern const char *sTagJournalId;
    extern const char *sTagLink;
    extern const char *sTagCategory;
    extern const char *sTagUserPic;
    extern const char *sTagUpdated;
    extern const char *sTagPosterId;
    extern const char *sTagAuthor;
    extern const char *sTagEntry;
    extern const char *sTagContent;

    // Rules file tags.
    extern const char *sTagRules;
    extern const char *sTagRule;
    extern const char *sTagFilter;
    extern const char *sTagValue;
    extern const char *sTagNameAttr;
    extern const char *sTagOption;

    // Defaults names.
    extern const char *MainWindowSize;
    extern const char *MainWindowPos;
    extern const char *MainWindowMax;
    extern const char *MainWindowState;
    extern const char *ShowStatusBar;
    extern const char *QuitOnClose;

    // Version related strings
    extern const char *sVersion;

    // SQL Queries

    /// SQL query to create table blogs.
    extern const char *sSqlCreateBlogTable;

    /// SQL query to create table posts.
    extern const char *sSqlCreatePostTable;

} // namespace str

#endif // __STRINGS_H__
