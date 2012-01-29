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

EventProperties::EventProperties()
{
    /*
    insert("admin_content_flag", QVariant());
    insert("adult_content", QVariant());
    insert("commentalter", QVariant());
    insert("current_coords", QVariant());
    insert("current_location", QVariant());
    insert("current_mood", QVariant());
    insert("current_moodid", QVariant());
    insert("current_music", QVariant());
    insert("hasscreened", QVariant());
    insert("opt_backdated", QVariant());
    insert("opt_nocomments", QVariant());
    insert("opt_noemail", QVariant());
    insert("opt_preformatted", QVariant());
    insert("opt_screening", QVariant());
    insert("personifi_lang", QVariant());
    insert("personifi_tags", QVariant());
    insert("personifi_word_count", QVariant());
    insert("picture_keyword", QVariant());
    insert("qotdid", QVariant());
    insert("revnum", QVariant());
    insert("revtime", QVariant());
    insert("sms_msgid", QVariant());
    insert("statusvis", QVariant());
    insert("syn_id", QVariant());
    insert("syn_link", QVariant());
    insert("taglist", QVariant());
    insert("unknown8bit", QVariant());
    insert("unsuspend_supportid", QVariant());
    insert("used_rte", QVariant());
    insert("useragent", QVariant());
    insert("verticals_list", QVariant());
    */
}

EventProperties::EventProperties(const QMap<QString, QVariant> &other)
{
    QMapIterator<QString, QVariant> it(other);
    while (it.hasNext()) {
        it.next();
        insert(it.key(), it.value());
    }
}

QMap<QString, QVariant> EventProperties::data() const
{
    return m_data;
}

void EventProperties::insert(const QString &key, const QVariant &value)
{
    m_data.insert(key, value);
}

QVariant &EventProperties::operator[](const QString &key)
{
    return m_data[key];
}

const QVariant EventProperties::operator[](const QString &key) const
{
    return m_data[key];
}

///////////////////////////////////////////////////////////////////////////////
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

            // Get event properties.
            event.m_properties = eventMap["props"].toMap();

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

Events Events::operator+(const Events &other) const
{
    Events tmp = *this;
    tmp.m_events += other.m_events;
    return tmp;
}

Events &Events::operator+=(const Events &other)
{
    m_events += other.m_events;
    return *this;
}

}
