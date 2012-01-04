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

#ifndef __LJSYNCITEMS_H__
#define __LJSYNCITEMS_H__

#include "export.h"
#include "response.h"

namespace lj
{

/// Implements the wrapper class around responce for the syncitems request.
class LJCOMMUNICATOR_EXPORT SyncItems : public xmlrpc::Response
{
public:
    SyncItems();
    SyncItems(const QByteArray &data);

    int count() const;
    int total() const;
    QString itemText(int index) const;
    QString time(int index) const;
    QString action(int index) const;

private:
    typedef QMap<QString, QVariant> Item;
    Item item(int index) const;

    QVariantList m_items;
    int m_count;
    int m_total;
};

} // namespace lj

#endif // __LJSYNCITEMS_H__


