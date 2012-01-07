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

#include "ljevents.h"

namespace lj
{

Events::Events()
{}

Events::Events(const QByteArray &data)
{
    // Parse response data and extract events list from
    // response map in case of valid result.
    QVariant response = parse(data);
    if (isValid()) {
        QMap<QString, QVariant> responseMap = response.toMap();
        m_events = responseMap["events"].toList();
    }
}

int Events::count() const
{
    return m_events.count();
}

QString Events::subject(int index) const
{
    return eventObj(index)["subject"].toString();
}

QString Events::event(int index) const
{
    return eventObj(index)["event"].toString();
}

QString Events::time(int index) const
{
    return eventObj(index)["eventtime"].toString();
}

int Events::commentCount(int index) const
{
    return eventObj(index)["reply_count"].toInt();
}

int Events::itemId(int index) const
{
    return eventObj(index)["itemid"].toInt();
}

int Events::publicId(int index) const
{
    Event e = eventObj(index);
    return e["itemid"].toInt() * 256 + e["anum"].toInt();
}

Events::Event Events::eventObj(int index) const
{
    if (index >= 0 && index < count())
        return m_events.at(index).toMap();
    else
        return Event();
}

}
