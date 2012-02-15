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

#ifndef __BLOGDATABASE_H__
#define __BLOGDATABASE_H__

#include "database.h"

namespace lj
{
    struct Event;
    class UserInfo;
    class UserTags;
    class FriendGroups;
}

namespace core
{

class BlogDatabase : public Database
{

public:
    BlogDatabase();

    /// Create database and its defaul tables.
    virtual bool create(const QString &fileName);

    /// Adds new blog event to the events table.
    void addEvent(const lj::Event &event);

    /// Sets the user data in the user table and password (encripted).
    /*!
        Before calling this function make sure that the user table already
        exists. We do not check for the table existence due to performance
        problems that can such check couse.
    */
    void setUserData(const lj::UserInfo &userInfo, const QString &password);

    /// Returns the last synced time as a string.
    QString lastSynced() const;

    /// Sets the last synced time.
    /*!
        The date-time expected to be in the format:
        yyyy-MM-dd hh:mm:ss.
    */
    void setLastSynced(const QString &lastSynced);

    /// Returns user's credentials.
    QString credentials() const;

    /// Returns user's moods as list of id, name and parent.
    QStringList moods() const;

    /// Returns journals user is member of.
    QStringList journals() const;

    /// Returns user's pics.
    QStringList userPics() const;

    /// Returns the backdated property value for the event at row.
    bool isBackdated(int id) const;

    /// Returns the event tags at row as a comma separated string.
    QString eventTags(int id) const;

    /// Returns the all available user tags as a comma separated string.
    QStringList userTags() const;

    void setUserTags(const lj::UserTags &tags);

    void setFriendGroups(const lj::FriendGroups &fg);

    QStringList securityNames() const;

    /// Returns user pic as a picture keyword.
    QString userPic(int id) const;

    /// Returns the event's current mood as a string.
    QString currentMood(int id) const;

    QStringList eventSecurity(int id) const;

    unsigned int eventMask(int id) const;

    QString location(int id) const;

private:
    /// Returns the event's given property value string.
    QString eventProperties(int row, const QString &name) const;
};

} // namespace core

#endif // __BLOGDATABASE_H__
