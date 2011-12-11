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
    extern const char *AppName;

    extern const char *FeedUrl;

    // XML Tags
    extern const char *TagFeed;
    extern const char *TagTitle;
    extern const char *TagName;
    extern const char *TagJournal;
    extern const char *TagJournalId;
    extern const char *TagLink;
    extern const char *TagCategory;
    extern const char *TagUserPic;
    extern const char *TagUpdated;
    extern const char *TagPosterId;
    extern const char *TagAuthor;
    extern const char *TagEntry;
    extern const char *TagContent;
    extern const char *TagEnabled;
    extern const char *TagRuleMatch;
    extern const char *TagRead;

    // Filters file tags.
    extern const char *TagFilters;
    extern const char *TagFilter;
    extern const char *TagRule;
    extern const char *TagValue;
    extern const char *TagNameAttr;
    extern const char *TagOption;
    extern const char *TagFolder;
    extern const char *TagVersion;

    // Defaults names.
    extern const char *MainWindowSize;
    extern const char *MainWindowPos;
    extern const char *MainWindowMax;
    extern const char *MainWindowState;
    extern const char *ShowStatusBar;
    extern const char *QuitOnClose;
    extern const char *Database;
    extern const char *Filters;
    extern const char *CheckUpdates;
    extern const char *Reconnect;
    extern const char *ReconnectCount;

    // Version related strings
    extern const char *Version;
    extern const char *VersionUrl;
    extern const char *FilterVersion;

    /// The name of blogs table. This should be unique.
    extern const char *BlogTableName;

    // SQL Queries

    /// SQL query to create table blogs.
    extern const char *SqlCreateBlogTable;

    /// SQL query to create table posts.
    extern const char *SqlCreatePostTable;

    /// SQL query to add post record to the table.
    extern const char *SqlInsertPostToTable;

    /// SQL query to insert blog record to the table.
    extern const char *SqlInsertBlogToTable;

} // namespace str

#endif // __STRINGS_H__
