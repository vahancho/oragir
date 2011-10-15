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

void Post::clear()
{
}

void Post::setTitle(const QString &title)
{
    m_title = title;
}

void Post::setUrl(const QString &url)
{
    m_url = url;
}

void Post::setTime(const QString &time)
{
    m_time = time;
}

void Post::setPosterName(const QString &name)
{
    m_posterName = name;
}

void Post::setPosterId(long id)
{
    m_posterId = id;
}

void Post::setUserPic(const QString &url)
{
    m_userPicUrl = url;
}

void Post::setContent(const QString &content)
{
    m_content = content;
}

void Post::addTag(const QString &tag)
{
    m_tags.push_back(tag);
}

} // namespace core
