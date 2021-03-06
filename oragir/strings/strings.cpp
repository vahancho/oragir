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
    const char *HelpUrl = "http://oragir.sourceforge.net/documentation.html";

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
    const char *TagRead = "read";

     // Filters file tags.
    const char *TagFilters = "filters";
    const char *TagFilter = "filter";
    const char *TagRule = "rule";
    const char *TagValue = "value";
    const char *TagNameAttr = "name";
    const char *TagOption = "option";
    const char *TagFolder = "folder";
    const char *TagVersion = "version";

    const char *MainWindowSize = "MainWindow/MainWindowSize";
    const char *MainWindowPos = "MainWindow/MainWindowPos";
    const char *MainWindowMax = "MainWindow/MainWindowMax";
    const char *MainWindowState = "MainWindow/MainWindowState";
    const char *ShowStatusBar = "MainWindow/ShowStatusBar";
    const char *QuitOnClose = "MainWindow/QuitOnClose";
    const char *Database = "Data/Database";
    const char *Filters = "Data/Filters";
    const char *CheckUpdates = "Version/CheckUpdates";
    const char *Reconnect = "Parser/Reconnect";
    const char *ReconnectCount = "Parser/ReconnectCount";
    
    // Version related strings
    const char *Version = "0.2.1 (beta)";
    const char *VersionUrl = "http://oragir.sourceforge.net/version";
    const char *FilterVersion = "1.0";

    const char *TimeFormat = "yyyy-MM-dd hh:mm:ss";

    const char *BlogTableName = "ad0ccb7f0813d3363f66b3fbc6b64eb66f680d8c";
    const char *MyBlogTableName = "a7a49a0485e8863dd34fbde8776e31cf9c9e3b63";

    // SQL Queries.
    const char *SqlCreateBlogTable = "CREATE TABLE IF NOT EXISTS %1 ("
                                     "journalid INTEGER PRIMARY KEY NOT NULL,"
                                     "link      VARCHAR(256)    NOT NULL,"
                                     "name      NVARCHAR(128)   NOT NULL,"
                                     "journal   VARCHAR(256)    NOT NULL,"
                                     "title     NVARCHAR(256),"
                                     "flag      INTEGER)";

    const char *SqlCreatePostTable = "CREATE TABLE IF NOT EXISTS %1 ("
                                     "flag     INTEGER,"
                                     "title    NVARCHAR(256),"
                                     "read     BOOLEAN,"
                                     "name     NVARCHAR(128)   NOT NULL,"
                                     "updated  DATETIME        NOT NULL,"
                                     "posterid INTEGER         NOT NULL,"
                                     "link     VARCHAR(256)    NOT NULL,"
                                     "content  NVARCHAR,"
                                     "userpic  NVARCHAR,"
                                     "category NVARCHAR)";

    const char *SqlCreateMyBlogTable = "CREATE TABLE IF NOT EXISTS %1 ("
                                       "itemid       INTEGER PRIMARY KEY NOT NULL,"
                                       "subject      NVARCHAR,"
                                       "event        NVARCHAR,"
                                       "publicid     INTEGER             NOT NULL,"
                                       "commentcount INTEGER             NOT NULL,"
                                       "time         DATETIME            NOT NULL,"                                       
                                       "url          NVARCHAR,"
                                       "security     NVARCHAR(128),"
                                       "allowmask    INTEGER,"
                                       "properties   NVARCHAR,"
                                       "flag         INTEGER)";

    extern const char *SqlCreateMyBlogUserTable = "CREATE TABLE IF NOT EXISTS user ("
                                                  "userid        INTEGER PRIMARY KEY NOT NULL,"
                                                  "password      NVARCHAR,"
                                                  "fullname      NVARCHAR,"
                                                  "usejournals   NVARCHAR,"
                                                  "defaultpicurl NVARCHAR,"
                                                  "pickwurls     NVARCHAR,"
                                                  "pickws        NVARCHAR,"
                                                  "moods         NVARCHAR,"
                                                  "tags          NVARCHAR,"
                                                  "friendgroups  NVARCHAR,"
                                                  "message       NVARCHAR,"
                                                  "lastsynced    DATETIME,"
                                                  "flag          INTEGER)";

    const char *SqlInsertPostToTable = "INSERT INTO %1 (flag, title, read, name, updated, posterid, link, content, userpic, category) "
                                       "VALUES (:flag, :title, :read, :name, :updated, :posterid, :link, :content, :userpic, :category)";

    const char *SqlInsertMyEntry = "INSERT OR REPLACE INTO %1 (itemid, subject, event, publicid, commentcount, time, url, security, allowmask, properties, flag) "
                                   "VALUES (:itemid, :subject, :event, :publicid, :commentcount, :time, :url, :security, :allowmask, :properties, :flag)";

    const char *SqlInsertBlogToTable = "INSERT INTO %1 (journalid, link, name, journal, title, flag) "
                                       "VALUES (:journalid, :link, :name, :journal, :title, :flag)";

    const char *SqlInsertMyBlogUserData = "INSERT OR REPLACE INTO user (userid, password, fullname, usejournals, defaultpicurl, pickwurls, pickws, moods, tags, friendgroups, message, lastsynced, flag) "
                                          "VALUES (:userid, :password, :fullname, :usejournals, :defaultpicurl, :pickwurls, :pickws, :moods, :tags, :friendgroups, :message, :lastsynced, :flag)";

} // namespace str
