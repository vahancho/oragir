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

#include "export.h"
#include "response.h"

namespace lj
{

/// Implements the wrapper class around responce for the events request.
class LJCOMMUNICATOR_EXPORT Events : public xmlrpc::Response
{
public:
    Events();
    Events(const QByteArray &data);

    typedef QMap<QString, QVariant> Event;

    /// Returns the number of events.
    int count() const;

    /// Returns event's (post) text.
    QString subject(int index) const;

    /// Returns event's (post) text.
    QString event(int index) const;

    /// Returs event's time.
    QString time(int index) const;

    /// Returns the number of event's comments.
    int commentCount(int index) const;

    /// Returns event id.
    int itemId(int index) const;

    /// Returns event's public id.
    /*!
        This parameter calculated by the following formula:
        publicId = itemId * 256 + anum
    */
    int publicId(int index) const;

    /// Returns the event's security.
    QString security(int index) const;

private:
    Event eventObj(int index) const;

    QVariantList m_events;
};

} // namespace lj

#endif // __LJEVENT_H__


