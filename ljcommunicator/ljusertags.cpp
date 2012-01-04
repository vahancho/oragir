/**************************************************************************
*   Copyright (C) 2012 by Vahan Aghajanyan                                *
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

#include "ljusertags.h"

namespace lj
{

UserTags::UserTags()
{}

UserTags::UserTags(const QByteArray &data)
{
    // Parse response data and extract events list from
    // response map in case of valid result.
    QVariant response = parse(data);
    if (isValid()) {
        QMap<QString, QVariant> responseMap = response.toMap();
        m_tags = responseMap["tags"].toList();
    }
}

int UserTags::count() const
{
    return m_tags.count();
}

QString UserTags::tagName(int index) const
{
    return tag(index)["name"].toString();
}

int UserTags::tagUses(int index) const
{
    return tag(index)["uses"].toInt();
}

UserTags::Tag UserTags::tag(int index) const
{
    if (index >= 0 && index < count())
        return m_tags.at(index).toMap();
    else
        return Tag();
}

}
