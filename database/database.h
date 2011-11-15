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

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <set>
#include <QtSql>
#include "../parser/blog.h"
#include "../parser/post.h"
#include "../parser/filter.h"

namespace core
{

class Database : public QObject
{
    Q_OBJECT
public:
    Database();

    ~Database();

    /// Creates database and returns result of creation.
    /*!
        If there is no active connection, the new connection
        become the active one, otherwise new connection does
        not replace existing active connection.
    */
    bool create(const QString &fileName);

    /// Closes, if open, and removes the given database connection.
    void remove(const QString &connectionName);

    /// Set the active (current) database.
    void setActive(const QString &connectionName);

    /// Adds new filter to the list of filters.
    void addFilter(const Filter<Post> &filter);

    bool saveFilters(const QString &fileName);
    bool openFilters(const QString &fileName);

    /// Reports the last error if any.
    QString errorMessage() const;

    QSqlDatabase database(const QString &connectionName) const;

    /// Returns true if given connection is the active one.
    bool isActive(const QString &connectionName) const;

    /// Returns the list of all database names.
    QStringList databases() const;

    typedef std::set<Filter<Post> > Filters;

    const Filters &filters() const;

signals:
    void recordInserted(const QSqlDatabase &db, const QString &table);

public slots:
    void onFetched(const Post &post, const Blog &blog);

private:
    /// Adds new recored to the database.
    void addRecord(const Post &post, const Blog &blog);

    /// Stores the list of filters.
    Filters m_filters;

    /// Stores the error message.
    QString m_error;

    QString m_dbActiveConnection;
};

} // namespace core

#endif // __DATABASE_H__
