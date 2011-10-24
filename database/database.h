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
#include "../parser/blog.h"
#include "../parser/post.h"
#include "../parser/rule.h"

namespace core
{

class Database : public QObject
{
    Q_OBJECT
public:
    Database();

    /// Creates database and returns result of creation.
    bool create();

    /// Adds new rule to the list of rules.
    void addRule(const Rule<Post> &rule);

    bool saveRules(const QString &fileName);
    bool openRules(const QString &fileName);

public slots:
    void onFetched(const Post &post, const Blog &blog);

private:
    /// Adds new recored to the database.
    void addRecord(const Post &post, const Blog &blog);

    /// Stores the list of rules.
    std::set<Rule<Post> > m_rules;
};

} // namespace core

#endif // __DATABASE_H__
