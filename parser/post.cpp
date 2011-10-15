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

#include "post.h"

namespace core
{

Post::Post()
    :
        m_posterId(-1)
{}

void Post::clear()
{
    m_title.clear();
    m_url.clear();
    m_time.clear();
    m_posterName.clear();
    m_posterId = -1;
    m_userPicUrl.clear();
    m_content.clear();
    m_tags.clear();
}

void Post::setTitle(const QString &title)
{
    m_title = title;
}

QString Post::title() const
{
    return m_title;
}

void Post::setUrl(const QString &url)
{
    m_url = url;
}

QString Post::url() const
{
    return m_url;
}

void Post::setTime(const QString &time)
{
    m_time = time;
}

QString Post::time() const
{
    return m_time;
}

void Post::setPosterName(const QString &name)
{
    m_posterName = name;
}

QString Post::posterName() const
{
    return m_posterName;
}

void Post::setPosterId(long id)
{
    m_posterId = id;
}

long Post::posterId() const
{
    return m_posterId;
}

void Post::setUserPic(const QString &url)
{
    m_userPicUrl = url;
}

QString Post::userPic() const
{
    return m_userPicUrl;
}

void Post::setContent(const QString &content)
{
    m_content = content;
}

QString Post::content() const
{
    return m_content;
}

void Post::addTag(const QString &tag)
{
    m_tags.push_back(tag);
}

QStringList Post::tags() const
{
    return m_tags;
}

} // namespace core
