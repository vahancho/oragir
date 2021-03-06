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

#include <set>
#include "../strings/strings.h"
#include "../../ljcommunicator/ljevents.h"
#include "../../ljcommunicator/ljuserinfo.h"
#include "../../ljcommunicator/ljusertags.h"
#include "../../ljcommunicator/ljfriendgroups.h"
#include "blogdatabase.h"

namespace core
{

const QString propSeparator("<>");

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
    q.bindValue(":allowmask", event.m_allowmask);
    q.bindValue(":flag", 0);

    // Concatinate the event's properties and their values into one
    // string and store it into the database.
    // All string items are separated by the given separator string.
    lj::EventProperties properties = event.m_properties;
    QMap<QString, QVariant> propData = properties.data();
    QMap<QString, QVariant>::const_iterator it = propData.constBegin();
    QString propStr;
    while (it != propData.constEnd()) {
        propStr.append(it.key());
        propStr.append(propSeparator);
        propStr.append(it.value().toString());
        propStr.append(propSeparator);
        ++it;
    }
    q.bindValue(":properties", propStr);

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
    q.bindValue(":tags", QString());
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

QStringList BlogDatabase::moods() const
{
    QSqlQuery q = query();
    q.prepare("SELECT moods FROM user");
    if (q.exec() && q.next())
        return q.value(0).toString().split(',');
    else
        return QStringList();
}

QStringList BlogDatabase::journals() const
{
    QSqlQuery q = query();
    q.prepare("SELECT usejournals FROM user");
    if (q.exec() && q.next())
        return q.value(0).toString().split(',');
    else
        return QStringList();
}

QStringList BlogDatabase::userPics() const
{
    QSqlQuery q = query();
    q.prepare("SELECT pickws FROM user");
    if (q.exec() && q.next())
        return q.value(0).toString().split(',');
    else
        return QStringList();
}

QString BlogDatabase::location(int id) const
{
    return eventProperties(id, "current_location");
}

bool BlogDatabase::isBackdated(int id) const
{
    return eventProperties(id, "opt_backdated") == "1";
}

QString BlogDatabase::eventTags(int id) const
{
    return eventProperties(id, "taglist");
}

lj::Security BlogDatabase::eventSecurity() const
{
    lj::Security security;

    QSqlQuery q = query();
    q.prepare("SELECT friendgroups FROM user");
    if (q.exec() && q.next()) {
        QStringList fg = q.value(0).toString().split(',');
        security.setFriendGroups(fg);
    }

    return security;
}

lj::Security BlogDatabase::eventSecurity(int id) const
{
    lj::Security security = eventSecurity();
    QSqlQuery q = query();
    QString qStr = QString("SELECT security FROM %1 WHERE itemid='%2'")
                           .arg(str::MyBlogTableName).arg(id);
    q.prepare(qStr);
    if (q.exec() && q.next()) {
        QString securityName = q.value(0).toString();

        if (securityName == "public") {
            security.setMajorSecurity("Public");
        } else if (securityName == "private") {
            security.setMajorSecurity("Private");
        } else if (securityName == "usemask") {
            quint32 mask = eventMask(id);
            security.setMask(mask);
        }
    }

    return security;
}

unsigned int BlogDatabase::eventMask(int id) const
{
    QSqlQuery q = query();
    QString qStr = QString("SELECT allowmask FROM %1 WHERE itemid='%2'")
                           .arg(str::MyBlogTableName).arg(id);
    q.prepare(qStr);
    if (q.exec() && q.next()) {
        return q.value(0).toUInt();
    }
    return -1;
}

QStringList BlogDatabase::userTags() const
{
    QSqlQuery q = query();
    q.prepare("SELECT tags FROM user");
    if (q.exec() && q.next())
        return q.value(0).toString().split(',');
    else
        return QStringList();
}

void BlogDatabase::setUserTags(const lj::UserTags &tags)
{
    QStringList tagList;
    if (tags.isValid()) {
        for (int i = 0; i < tags.count(); ++i) {
            tagList.push_back(tags.tagName(i));
        }
    }
    // Sort tags before storing them.
    qSort(tagList);
    QSqlQuery q = query();
    q.prepare(QString("UPDATE user SET tags = '%1'").arg(tagList.join(",")));
    q.exec();
}

void BlogDatabase::setFriendGroups(const lj::FriendGroups &fg)
{
    QStringList fgList;
    if (fg.isValid()) {
        for (int i = 0; i < fg.count(); ++i) {
            fgList.push_back(QString::number(fg.groupId(i)));
            fgList.push_back(fg.groupName(i));
        }
    }
    QSqlQuery q = query();
    q.prepare(QString("UPDATE user SET friendgroups = '%1'")
              .arg(fgList.join(",")));
    q.exec();
}

QString BlogDatabase::userPic(int id) const
{
    return eventProperties(id, "picture_keyword");
}

QString BlogDatabase::currentMood(int id) const
{
    int moodId = eventProperties(id, "current_moodid").toInt();
    QStringList moodsList = moods();
    for (int i = 0; i < moodsList.count(); i += 3) {
        if (moodsList.at(i).toInt() == moodId) {
            return moodsList.at(i + 1);
        }
    }
    return QString();
}

QString BlogDatabase::eventProperties(int id, const QString &name) const
{
    QSqlQuery q = query();
    QString qStr = QString("SELECT properties FROM %1 WHERE itemid='%2'")
                           .arg(str::MyBlogTableName).arg(id);
    q.prepare(qStr);
    if (q.exec() && q.next()) {
        QStringList propList = q.value(0).toString().split(propSeparator);
        for (int i = 0; i < propList.size(); i += 2) {
            if (propList.at(i) == name) {
                return propList.at(i + 1);
            }
        }
    }
    return QString();
}

} // namespace core
