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
    const char *sAppName = "Oragiro";
    
    const char *sFeedUrl = "http://atom.services.livejournal.com/atom-stream.xml";

    // XML Tags
    const char *sTagFeed = "feed";
    const char *sTagTitle = "title";
    const char *sTagAuthor = "author";
    const char *sTagEntry = "entry";
    const char *sTagContent = "content";    
    const char *sTagName = "name";
    const char *sTagJournal = "journal";
    const char *sTagJournalId = "journalid";
    const char *sTagLink = "link";
    const char *sTagCategory = "category";
    const char *sTagUserPic = "userpic";
    const char *sTagUpdated = "updated";
    const char *sTagPosterId = "posterid";

     // Rules file tags.
    const char *sTagRules = "rules";
    const char *sTagRule = "rule";
    const char *sTagFilter = "filter";
    const char *sTagValue = "value";
    const char *sTagNameAttr = "name";
    const char *sTagOption = "option";

    const char *MainWindowSize = "MainWindowSize";
    const char *MainWindowPos = "MainWindowPos";
    const char *MainWindowMax = "MainWindowMax";
    const char *MainWindowState = "MainWindowState";
    const char *ShowStatusBar = "ShowStatusBar";
    const char *QuitOnClose = "QuitOnClose";
    
    // Version related strings
    const char *sVersion = "0.0.1";

    // SQL Queries.
    const char *sSqlCreateBlogTable = "CREATE TABLE IF NOT EXISTS blog ("
                                      "journalid INT PRIMARY KEY NOT NULL,"
                                      "link      VARCHAR(256)    NOT NULL,"
                                      "name      NVARCHAR(128)   NOT NULL,"
                                      "journal   VARCHAR(256)    NOT NULL,"
                                      "title     NVARCHAR(256))";

    const char *sSqlCreatePostTable = "CREATE TABLE IF NOT EXISTS post ("
                                      "posterid INT             NOT NULL,"
                                      "link     VARCHAR(256)    NOT NULL,"
                                      "updated  DATATIME        NOT NULL,"
                                      "name     NVARCHAR(128)   NOT NULL,"
                                      "content  NVARCHAR,"
                                      "title    NVARCHAR(256))";

} // namespace str
