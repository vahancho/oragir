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

#include <QtSql>
#include <QMessageBox>
#include "database.h"

namespace core
{

Database::Database()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
     QMessageBox::critical(0, "Cannot open database",
                              "Unable to establish a database connection.\n"
                              "This example needs SQLite support. Please read "
                              "the Qt SQL driver documentation for information how "
                              "to build it.", QMessageBox::Cancel);
    }

    QSqlQuery query;
    query.exec("CREATE TABLE blog ("
               "journalid INT PRIMARY KEY NOT NULL,"
               "link      VARCHAR(256)    NOT NULL,"
               "name      NVARCHAR(128)   NOT NULL,"
               "journal   VARCHAR(256)    NOT NULL,"
               "title     NVARCHAR(256))");

    query.exec("CREATE TABLE entry ("
               "posterid INT             NOT NULL,"
               "link     VARCHAR(256)    NOT NULL,"
               "updated  DATATIME        NOT NULL,"
               "name     NVARCHAR(128)   NOT NULL,"
               "content  NVARCHAR,"
               "title    NVARCHAR(256))");
}

void Database::onFetched(const Blog &blog)
{
}

void Database::onFetched(const Post &post)
{
}



} // namespace core
