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

#include <QtXml>
#include "request.h"
#include "converter.h"

namespace xmlrpc
{

Request::Request()
{}

QByteArray Request::compose(const QString &methodName,
                            const QVariantList &parameters) const
{
    QDomDocument doc;
    QDomElement methodCall = doc.createElement("methodCall");
    doc.appendChild( methodCall );

    QDomElement methodNameNode = doc.createElement("methodName");
    methodNameNode.appendChild( doc.createTextNode( methodName ) );
    methodCall.appendChild( methodNameNode );

    QDomElement paramsNode = doc.createElement("params");
    methodCall.appendChild( paramsNode );

    foreach (const QVariant &parameter, parameters) {
        QDomElement paramNode = doc.createElement("param");
        paramNode.appendChild( Converter::toDomElement(parameter, doc) );
        paramsNode.appendChild( paramNode );
    }

    return doc.toByteArray(2);
}

} // namespace xmlrpc