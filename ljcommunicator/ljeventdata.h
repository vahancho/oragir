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

#ifndef __LJEVENTDATA_H__
#define __LJEVENTDATA_H__

#include "export.h"
#include "response.h"

namespace lj
{

/// Implements the wrapper class around response for the LJ.XMLRPC.postevent(editevent) request.
class LJCOMMUNICATOR_EXPORT EventData : public xmlrpc::Response
{
public:
    EventData(const QByteArray &data);

    int id() const;
    int anum() const;
    QString url() const;

private:
    QMap<QString, QVariant> m_data;
};

} // namespace lj

#endif // __LJEVENTDATA_H__


