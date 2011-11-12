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

namespace str
{
    //////////////////////////////////////////////////////////////////////////
    // Not Translatable Strings
    //

    // Application name
    const char *AppName = "Oragir";
    
    const char *FeedUrl = "http://atom.services.livejournal.com/atom-stream.xml";

    // XML Tags
    const char *TagFeed = "feed";
    const char *TagTitle = "title";
    const char *TagAuthor = "author";
    const char *TagEntry = "entry";
    const char *TagContent = "content";
    const char *TagName = "name";
    const char *TagJournal = "journal";
    const char *TagJournalId = "journalid";
    const char *TagLink = "link";
    const char *TagCategory = "category";
    const char *TagUserPic = "userpic";
    const char *TagUpdated = "updated";
    const char *TagPosterId = "posterid";
    const char *TagEnabled = "enabled";
    const char *TagRuleMatch = "match";

     // Filters file tags.
    const char *TagFilters = "filters";
    const char *TagFilter = "filter";
    const char *TagRule = "rule";
    const char *TagValue = "value";
    const char *TagNameAttr = "name";
    const char *TagOption = "option";

    const char *MainWindowSize = "MainWindowSize";
    const char *MainWindowPos = "MainWindowPos";
    const char *MainWindowMax = "MainWindowMax";
    const char *MainWindowState = "MainWindowState";
    const char *ShowStatusBar = "ShowStatusBar";
    const char *QuitOnClose = "QuitOnClose";
    const char *Databases = "Databases";
    const char *Filters = "Filters";
    
    // Version related strings
    const char *Version = "0.1a";

    // SQL Queries.
    const char *SqlCreateBlogTable = "CREATE TABLE IF NOT EXISTS blog ("
                                     "journalid INTEGER PRIMARY KEY NOT NULL,"
                                     "link      VARCHAR(256)    NOT NULL,"
                                     "name      NVARCHAR(128)   NOT NULL,"
                                     "journal   VARCHAR(256)    NOT NULL,"
                                     "title     NVARCHAR(256))";

    const char *SqlCreatePostTable = "CREATE TABLE IF NOT EXISTS post ("
                                     "posterid INTEGER         NOT NULL,"
                                     "link     VARCHAR(256)    NOT NULL,"
                                     "updated  DATATIME        NOT NULL,"
                                     "name     NVARCHAR(128)   NOT NULL,"
                                     "content  NVARCHAR,"
                                     "title    NVARCHAR(256))";

} // namespace str
