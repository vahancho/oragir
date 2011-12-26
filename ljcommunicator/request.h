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

#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <QVariant>

namespace xmlrpc
{

class Request
{
public:
    Request();

    /// Creates and returns the <methodCall> XMLRPC structure.
    /*!
        The <methodCall> must contain a <methodName> sub-item, a string,
        containing the name of the method to be called. The string may
        only contain identifier characters, upper and lower-case A-Z,
        the numeric characters, 0-9, underscore, dot, colon and slash.
        It's entirely up to the server to decide how to interpret the
        characters in a methodName.
        If the procedure call has parameters, the <methodCall> must
        contain a <params> sub-item. The <params> sub-item can contain
        any number of <param>s, each of which has a <value>.
    */
    QByteArray compose(const QString &methodName,
                       const QVariantList &parameters) const;
};

} // namespace xmlrpc

#endif // __REQUEST_H__


