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

namespace xmlrpc
{

// xmlrpc scalar types. Refer to the spec (http://xmlrpc.scripting.com/spec.html)
const char tagInt[] = "int";
const char tagBool[] = "boolean";              // 0 (false) or 1 (true)
const char tagString[] = "string";
const char tagDouble[] = "double";             // Double-precision signed floating point number
const char tagDateTime[] = "dateTime.iso8601"; // date/time 19980717T14:08:55
const char tagBase64[] = "base64";             // base64-encoded binary

// Other tags
const char tagValue[] = "value";
const char tagArray[] = "array";
const char tagStruct[] = "struct";
const char tagData[] = "data";
const char tagMember[] = "member";
const char tagName[] = "name";
const char tagMethodCall[] = "methodCall";
const char tagMethodName[] = "methodName";
const char tagParams[] = "params";
const char tagParam[] = "param";

Request::Request()
{}

QByteArray Request::compose(const QString &methodName,
                            const QVariantList &parameters) const
{
    QDomDocument document;
    QDomElement methodCall = document.createElement(tagMethodCall);
    document.appendChild( methodCall );

    QDomElement methodNameNode = document.createElement(tagMethodName);
    methodNameNode.appendChild(document.createTextNode(methodName));
    methodCall.appendChild(methodNameNode);

    QDomElement paramsNode = document.createElement(tagParams);
    methodCall.appendChild(paramsNode);

    foreach (const QVariant &parameter, parameters) {
        QDomElement paramNode = document.createElement(tagParam);
        paramNode.appendChild(toDomElement(parameter, document));
        paramsNode.appendChild(paramNode);
    }

    return document.toByteArray();
}

QDomElement Request::toDomElement(const QVariant &val, QDomDocument &doc) const
{
    QDomElement valueElement = doc.createElement(tagValue);
    QDomElement data;

    switch (val.type()) {
    case QVariant::Int:
    case QVariant::UInt:
        data = doc.createElement(tagInt);
        data.appendChild(doc.createTextNode(val.toString()));
        break;
    case QVariant::String:
        data = doc.createElement(tagString);
        data.appendChild(doc.createTextNode(val.toString()));
        break;
    case QVariant::Double:
        data = doc.createElement(tagDouble);
        data.appendChild(doc.createTextNode(val.toString()));
        break;
    case QVariant::DateTime:
        data = doc.createElement(tagDateTime);
        data.appendChild(doc.createTextNode(toDateTime(val.toDateTime())));
        break;
    case QVariant::Bool:
        data = doc.createElement(tagBool);
        data.appendChild(doc.createTextNode(val.toBool() ? "1" : "0"));
        break;
    case QVariant::ByteArray:
        data = doc.createElement(tagBase64);
        data.appendChild(doc.createTextNode(val.toByteArray().toBase64()));
        break;
    case QVariant::List:
    case QVariant::StringList:
        {
            data = doc.createElement(tagArray);
            QDomElement arrayData = doc.createElement(tagData);
            data.appendChild( arrayData );
            QList<QVariant> list = val.toList();
            QList<QVariant>::iterator it;
            for (it = list.begin(); it!=list.end(); ++it) {
                // Recursion
                arrayData.appendChild(toDomElement((*it), doc));
            }
            break;
        }
    case QVariant::Map:
        {
            data = doc.createElement(tagStruct);
            QMap<QString, QVariant> map = val.toMap();
            QMap<QString, QVariant>::ConstIterator it;
            for (it = map.constBegin(); it != map.constEnd(); ++it) {
                QDomElement member = doc.createElement(tagMember);
                data.appendChild(member);

                QDomElement nameElement = doc.createElement(tagName);
                nameElement.appendChild(doc.createTextNode(it.key()));
                // Recursion
                QDomElement valueElement = toDomElement(it.value(), doc);

                member.appendChild(nameElement);
                member.appendChild(valueElement);
            }
            break;
        }
    default:
        {
            // unsupported type
        }
    };

    if (!data.isNull()) {
        valueElement.appendChild(data);
    }

    return valueElement;
}

QString Request::toDateTime(const QDateTime &date) const
{
    return date.toString("yyyyMMddThh:mm:ss");
}

} // namespace xmlrpc