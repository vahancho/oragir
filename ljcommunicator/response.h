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
#include "export.h"

namespace xmlrpc
{

class LJCOMMUNICATOR_EXPORT Response
{

public:
    Response();

    /// Parses the given response and check for its correctness.
    QVariant parse(const QByteArray &response);

    bool isValid() const;
    QString error() const;

private:
    QVariant value(const QDomElement &node);

    QDateTime fromDateTime(const QString &datetime);

    /// Parses <struct> element and returns result.
    /*!
        <struct>s can be recursive, any <value> may contain a <struct> or
        any other type, including an <array>.
        Struct elements structure looks like:
        <struct>
            <member>
                <name>someName</name>
                <value><i4>18</i4></value>
            </member>
            <member>
                <name>anotherName</name>
                <value><i4>139</i4></value>
            </member>
       </struct>
    */
    QVariant fromStruct(const QDomElement &node);

    /// Parses <array> elements and return result - QVariantList.
    /*!
        <array> elements do not have names.
        Example:
        <array>
            <data>
                <value><i4>12</i4></value>
                <value><string>Egypt</string></value>
                <value><boolean>0</boolean></value>
                <value><i4>-31</i4></value>
            </data>
       </array>

       <arrays>s can be recursive, any value may contain an <array> or
       any other type, including a <struct>.
    */
    QVariant fromArray(const QDomElement& node);

    QString m_errorString;
    bool m_isValid;
};

} // namespace xmlrpc

#endif // __RESPONSE_H__

