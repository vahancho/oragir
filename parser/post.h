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

#ifndef __POST_H__
#define __POST_H__

#include <QStringList>

namespace core
{

class Post
{
public:
    Post();

    void clear();

    void setTitle(const QString &title);
    QString title() const;

    void setUrl(const QString &url);
    QString url() const;

    void setTime(const QString &time);
    QString time() const;

    void setPosterName(const QString &name);
    QString posterName() const;

    void setPosterId(long id);
    long posterId() const;

    void setUserPic(const QString &url);
    QString userPic() const;

    void setContent(const QString &content);
    QString content() const;

    void addTag(const QString &tag);
    QStringList tags() const;

private:
    QString m_title;
    QString m_url;
    QString m_time;
    QString m_posterName;
    long m_posterId;
    QString m_userPicUrl;
    QString m_content;
    QStringList m_tags;    
};

} // namespace core

#endif // __POST_H__
