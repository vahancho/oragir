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

#ifndef __STREAMDATABASE_H__
#define __STREAMDATABASE_H__

#include <set>
#include "database.h"
#include "../parser/blog.h"
#include "../parser/post.h"
#include "../parser/filter.h"

namespace core
{

class StreamDatabase : public QObject, public Database
{
    Q_OBJECT
public:
    StreamDatabase();

    bool addStreamTable(const QString &table);

    /// Adds or update a filter to the list of filters.
    /*!
        This function either adds a new filter to container
        or replace existing one (with the same name).
    */
    void addFilter(const Filter<Post> &filter);

    bool saveFilters(const QString &fileName);

    /// Loades new filters files.
    /*!
        Callee should take care of backing up existing filters and restore
        them in case of errors in loading new file.
    */
    bool openFilters(const QString &fileName);

    void clearFilters();

    typedef std::set<Filter<Post> > Filters;

    /// Returns the filters list.
    const Filters &filters() const;

    int unreadCount(const QString &table) const;
    int totalCount(const QString &table) const;

signals:
    void recordInserted(const QString &table);
    void tableAdded(const QString &table);

public slots:
    void onFetched(const Post &post, const Blog &blog);

private:
    /// Adds new recors to the given table.
    void addRecord(const Post &post, const Blog &blog,
                   const QString &table);

    /// Stores the list of filters.
    Filters m_filters;
};

} // namespace core

#endif // __STREAMDATABASE_H__
