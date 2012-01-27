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
    void setUserData(const lj::UserInfo &userInfo, const QString &password,
                     const QDateTime &lastsynced);

    /// Returns the last synced time as a string.
    QString lastSynced() const;
};

} // namespace core

#endif // __BLOGDATABASE_H__
