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

#ifndef __BLOG_H__
#define __BLOG_H__

#include <QList>
#include "post.h"

namespace core
{

class Blog
{
public:
    Blog();

    void clear();

    void setTitle(const QString &title);
    void setUrl(const QString &url);
    void setName(const QString &name);
    void setJournal(const QString &journal);
    void setId(long id);

private:
    QString m_title;
    QString m_url;
    QString m_name;
    QString m_journal;
    long m_id;
};

} // namespace core

#endif // __BLOG_H__
