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

#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include "converter.h"

namespace xmlrpc
{

// xmlrpc scalar types. Refer to the spec (http://xmlrpc.scripting.com/spec.html)
const char tagInt[] = "int";
const char tagInt2[] = "i4";
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

QDomElement Converter::toDomElement(const QVariant &val, QDomDocument &doc)
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
                arrayData.appendChild(toDomElement((*it), doc));
            }
            break;
        }
    case QVariant::Map:
        {
            data = doc.createElement(tagStruct);
            QMap<QString,QVariant> map = val.toMap();
            QMap<QString,QVariant>::ConstIterator it;
            for (it = map.constBegin(); it != map.constEnd(); ++it) {
                QDomElement member = doc.createElement(tagMember);
                data.appendChild(member);

                QDomElement name = doc.createElement(tagName);
                name.appendChild(doc.createTextNode(it.key()));
                QDomElement value = toDomElement(it.value(), doc);

                member.appendChild( name );
                member.appendChild( value );
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

QVariant Converter::fromDomElement(const QDomElement &node)
{
    Q_ASSERT(node.nodeName() == tagValue);

    if (node.nodeName() == tagValue) {
        QDomElement data = node.firstChild().toElement();
        if (data.isNull()) {
            return node.text();
        } else {
            QString tagName = data.tagName();
            QString tagText = data.text();

            if (tagName == tagInt || tagName == tagInt2) {
                return tagText.toInt();
            } else if (tagName == tagString) {
                return tagText;
            } else if (tagName == tagDouble) {
                return tagText.toDouble();
            } else if (tagName == tagBase64) {
                // Decode from base64 and than from utf-8.
                QByteArray ba = QByteArray::fromBase64(tagText.toAscii());
                return QString::fromUtf8(ba.data());
            } else if (tagName == tagBool) {
                return bool(tagText.toInt() == 1);
            } else if (tagName == tagDateTime) {
                return fromDateTime(tagText);
            } else if (tagName == tagStruct) {
                return fromStruct(data);
            } else if (tagName == tagArray) {
                return fromArray( data );
            } else {
                return QVariant::Invalid;
            }
        }
    } else {
        return QVariant::Invalid;
    }
}

QString Converter::toDateTime(const QDateTime &date)
{
    return date.toString("yyyyMMddThh:mm:ss");
}

QDateTime Converter::fromDateTime(const QString &sdate)
{
    QDateTime res = QDateTime::fromString(sdate, Qt::ISODate);
    return res;
}

QVariant Converter::fromStruct(const QDomElement &node)
{
    Q_ASSERT(node.tagName() == tagStruct);

    QMap<QString, QVariant> res;
    QDomElement member = node.firstChild().toElement();
    while (!member.isNull()) {
        Q_ASSERT(member.tagName() == tagMember);

        QString name;
        QVariant value;
        QDomElement child = member.firstChild().toElement();
        while (!child.isNull()) {
            QString tag = child.tagName();
            if (tag == tagName) {
                name = child.text();
            } else if (tag == tagValue) {
                value = fromDomElement( child );
            } else {
                // Unexpected tag name.
                return QVariant::Invalid;
            }

            child = child.nextSibling().toElement();
        }

        if (!name.isNull() && value.isValid()) {
            res[name] = value;
        }

        member = member.nextSibling().toElement();
    }

    return res;
}

QVariant Converter::fromArray(const QDomElement& node)
{
    Q_ASSERT(node.tagName() == tagArray);
    Q_ASSERT(node.firstChildElement().tagName() == tagData);
    QVariantList res;
    QDomElement data = node.firstChildElement().firstChildElement();

    while (!data.isNull()) {
        if (data.tagName() == tagValue) {
            res.append(fromDomElement(data));
        } else {
            // Unexpected tag name.
            return QVariant::Invalid;
        }
        data = data.nextSibling().toElement();
    }

    return res;
}

}
