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

#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <QVariant>
#include <QDomElement>

namespace xmlrpc
{

class Response
{

public:
    Response(const QByteArray &response);

    QVariant data() const;
    bool isValid() const;

private:
    /// Parses the given response and check for its correctness.
    bool parse(const QByteArray &response);

    QVariant m_data;
    QString m_errorString;
    bool m_isValid;
};

} // namespace xmlrpc

#endif // __RESPONSE_H__

