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

#include "../strings/strings.h"
#include "../../ljcommunicator/ljevents.h"
#include "../../ljcommunicator/ljuserinfo.h"
#include "blogdatabase.h"

namespace core
{

BlogDatabase::BlogDatabase()
{}

bool BlogDatabase::create(const QString &fileName)
{
    if (Database::create(fileName)) {
        // Create new table for the blog events if it does not exist.
        // No problem if the table already exists.
        QString query = QString(str::SqlCreateMyBlogTable)
                                .arg(str::MyBlogTableName);
        addTable(query);

        // Create the user info table and add user data.
        addTable(str::SqlCreateMyBlogUserTable);

        return true;
    } else {
        return false;
    }
}

void BlogDatabase::addEvent(const lj::Event &event)
{
    QSqlQuery q = query();

    QString queryStr = QString(str::SqlInsertMyEntry).arg(str::MyBlogTableName);
    q.prepare(queryStr);
    q.bindValue(":itemid", event.m_itemId);
    q.bindValue(":subject", event.m_subject);
    q.bindValue(":event", event.m_event);
    q.bindValue(":publicid", event.m_publicId);
    q.bindValue(":commentcount", event.m_commentCount);
    q.bindValue(":time", event.m_time);
    q.bindValue(":url", event.m_url);
    q.bindValue(":security", event.m_security);
    q.bindValue(":flag", 0);

    lj::EventProperties properties = event.m_properties;
    q.bindValue(":tags", properties["taglist"].toStringList().join(","));
    q.bindValue(":backdated", properties["opt_backdated"].toBool());

    q.exec();
}

void BlogDatabase::setUserData(const lj::UserInfo &userInfo,
                               const QString &password)
{
    QSqlQuery q = query();

    QString queryStr = QString(str::SqlInsertMyBlogUserData);
    q.prepare(queryStr);
    q.bindValue(":userid", userInfo.id());
    q.bindValue(":password", password);
    q.bindValue(":fullname", userInfo.fullName());
    q.bindValue(":usejournals", userInfo.journals().join(","));
    q.bindValue(":defaultpicurl", userInfo.defaultPicUrl());
    q.bindValue(":pickwurls", userInfo.pictureUrls().join(","));
    q.bindValue(":pickws", userInfo.pictureKeys().join(","));
    q.bindValue(":moods", userInfo.moods().join(","));
    q.bindValue(":friendgroups", QString());
    q.bindValue(":message", userInfo.message());

    // Use the dafault time that is earlier than even possible.
    q.bindValue(":lastsynced", QString("1900-01-01 00:00:00"));
    q.bindValue(":flag", 0);

    q.exec();
}

QString BlogDatabase::lastSynced() const
{
    QSqlQuery q = query();
    q.prepare("SELECT lastsynced FROM user");
    if (q.exec() && q.next())
        return q.value(0).toString();
    else
        return "1900-01-01 00:00:00";
}

void BlogDatabase::setLastSynced(const QString &lastSynced)
{
    QSqlQuery q = query();
    q.prepare(QString("UPDATE user SET lastsynced = '%1'").arg(lastSynced));
    q.exec();
}

QString BlogDatabase::credentials() const
{
    QSqlQuery q = query();
    q.prepare("SELECT password FROM user");
    if (q.exec() && q.next())
        return q.value(0).toString();
    else
        return QString();
}

} // namespace core
