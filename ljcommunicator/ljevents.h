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

#ifndef __LJEVENT_H__
#define __LJEVENT_H__

#include <QStringList>
#include "export.h"
#include "response.h"

namespace lj
{

/// Implements event properties list.
/*!
    Refer the LJ docs: http://www.livejournal.com/doc/server/ljp.csp.proplist.html
*/
class LJCOMMUNICATOR_EXPORT EventProperties
{
public:
    EventProperties();
    EventProperties(const QMap<QString, QVariant> &other);

    QMap<QString, QVariant> data() const;

    QVariant &operator[](const QString &key);
    const QVariant operator[](const QString &key) const;

private:
    void insert(const QString &key, const QVariant &value);
    QMap<QString, QVariant> m_data;
};

struct Event
{
    Event()
        :
            m_itemId(0),
            m_publicId(0),
            m_commentCount(0)
    {}

    int m_itemId;
    int m_publicId;
    int m_commentCount;
    QString m_subject;
    QString m_event;
    QString m_url;
    QString m_time;
    QString m_security;
    EventProperties m_properties;
};

/// Implements the wrapper class around responce for the events request.
class LJCOMMUNICATOR_EXPORT Events : public xmlrpc::Response
{
public:
    Events();
    Events(const QByteArray &data);

    /// Returns the number of events.
    int count() const;

    /// Returns the event structure.
    Event event(int index);

    Events operator+(const Events &other) const;
    Events &operator+=(const Events &other);

private:
    QList<Event> m_events;
};

} // namespace lj

#endif // __LJEVENT_H__


