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

#include "ljfriendgroups.h"

namespace lj
{

FriendGroups::FriendGroups()
{}

FriendGroups::FriendGroups(const QByteArray &data)
{
    // Parse response data and extract events list from
    // response map in case of valid result.
    QVariant response = parse(data);
    if (isValid()) {
        QMap<QString, QVariant> responseMap = response.toMap();
        m_groups = responseMap["friendgroups"].toList();
    }
}

int FriendGroups::count() const
{
    return m_groups.count();
}

QString FriendGroups::groupName(int index) const
{
    return group(index)["name"].toString();
}

int FriendGroups::groupId(int index) const
{
    return group(index)["id"].toInt();
}

int FriendGroups::groupSortOrder(int index) const
{
    return group(index)["sortorder"].toInt();
}

bool FriendGroups::groupPublic(int index) const
{
    return group(index)["public"].toBool();
}

FriendGroups::Group FriendGroups::group(int index) const
{
    if (index >= 0 && index < count())
        return m_groups.at(index).toMap();
    else
        return Group();
}

}
