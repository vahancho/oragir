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

#include <QStringList>
#include "../strings/strings.h"
#include "post.h"

namespace core
{

Post::Post()
{
    addProperty(str::TagTitle, QString());
    addProperty(str::TagLink, QString());
    addProperty(str::TagUpdated, QString());
    addProperty(str::TagName, QString());
    addProperty(str::TagUserPic, QString());
    addProperty(str::TagContent, QString());
    addProperty(str::TagCategory, QStringList());
    addProperty(str::TagPosterId, -1L);
}

void Post::addTag(const QString &tag)
{
    QStringList tags = value(str::TagCategory).toStringList();
    tags.push_back(tag);
    setValue(str::TagCategory, tags);
}

} // namespace core
