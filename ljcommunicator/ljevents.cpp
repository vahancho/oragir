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
        QVariantList events = responseMap["events"].toList();
        foreach (const QVariant &v, events) {
            QMap<QString, QVariant> eventMap = v.toMap();

            Event event;
            event.m_itemId = eventMap["itemid"].toInt();
            event.m_publicId = eventMap["itemid"].toInt() * 256 + eventMap["anum"].toInt();
            event.m_commentCount = eventMap["reply_count"].toInt();
            event.m_subject = eventMap["subject"].toString();
            event.m_event = eventMap["event"].toString();
            event.m_url = eventMap["url"].toString();
            event.m_time = eventMap["eventtime"].toString();

            if (!eventMap.contains("security")) {
                event.m_security = "public";
            } else {
                event.m_security = eventMap["security"].toString();
            }

            QMap<QString, QVariant> properties = eventMap["props"].toMap();
            event.m_tags = properties["taglist"].toStringList();

            m_events.push_back(event);
        }
    }
}

int Events::count() const
{
    return m_events.count();
}

Event Events::event(int index)
{
    if (index >= 0 && index < count())
        return m_events.at(index);
    else
        return Event();
}

}
