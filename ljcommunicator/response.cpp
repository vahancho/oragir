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
#include "response.h"

namespace xmlrpc
{

const char tagParams[] = "params";
const char tagParam[] = "param";
const char tagValue[] = "value";
const char tagFault[] = "fault";
const char tagArray[] = "array";
const char tagStruct[] = "struct";
const char tagData[] = "data";
const char tagMember[] = "member";
const char tagName[] = "name";
const char tagFaultCode[] = "faultCode";
const char tagFaultString[] = "faultString";

// xmlrpc scalar types. Refer to the spec (http://xmlrpc.scripting.com/spec.html)
const char tagInt[] = "int";
const char tagInt2[] = "i4";
const char tagBool[] = "boolean";              // 0 (false) or 1 (true)
const char tagString[] = "string";
const char tagDouble[] = "double";             // Double-precision signed floating point number
const char tagDateTime[] = "dateTime.iso8601"; // date/time 19980717T14:08:55
const char tagBase64[] = "base64";             // base64-encoded binary

Response::Response()
    :
        m_isValid(false)
{}

bool Response::isValid() const
{
    return m_isValid;
}

QVariant Response::parse(const QByteArray &response)
{
    QVariant res;
    QDomDocument doc;
    int domErrorLine;
    int domErrorColumn;

    if (!doc.setContent(response, &m_errorString, &domErrorLine, &domErrorColumn)) {
        m_isValid = false;
    }

    // From the xmlrpc spec: The body of the response is a single XML structure,
    // a <methodResponse>, which can contain a single <params> which 
    // contains a single <param> which contains a single <value>.
    QDomElement methodResponse = doc.firstChildElement("methodResponse");

    if (methodResponse.isNull()) {
        m_errorString = "XMLRPC format error: missed methodResponse tag";
        m_isValid = false;
    }

    QDomElement subNode = methodResponse.firstChildElement();

    // The <methodResponse> could also contain a <fault> which contains
    // a <value> which is a <struct> containing two elements, one named
    // <faultCode>, an <int> and one named <faultString>, a <string>.
    // A <methodResponse> can not contain both a <fault> and a <params>.
    QString tagName = subNode.tagName();

    if (tagName == tagParams) {
        // <params> contains a single <param> which contains a single <value>.
        QDomElement paramElement = subNode.firstChild().toElement();
        tagName = paramElement.tagName();
        if (tagName != tagParam) {
            m_errorString = QString("XMLRPC format error: expected %1, found %2")
                                    .arg(tagParam)
                                    .arg(tagName);
            m_isValid = false;
        }

        QDomElement valueElement = paramElement.firstChild().toElement();
        tagName = valueElement.tagName();
        if (tagName != tagValue) {
            m_errorString = QString("XMLRPC format error: expected %1, found %2")
                                    .arg(tagValue)
                                    .arg(tagName);
            m_isValid = false;
        }

        // Read param data.
        res = value(valueElement);

        if (!res.isValid()) {
            m_isValid = false;
        }
    } else if (tagName == tagFault ) {
        QDomElement paramValue = subNode.firstChildElement();

        // Should be QMap
        QVariant res = value(paramValue);
        if (res.type() != QVariant::Map) {
            m_errorString = QString("XMLRPC format error: responce fault "
                                    "value is not a struct");
            m_isValid = false;
        }

        QMap<QString, QVariant> map = res.toMap();
        if (!map.contains(tagFaultCode) || !map.contains(tagFaultString)) {
            m_errorString = "XMLRPC format error: Fault struct should contain "
                            "either fault code or fault description";
            m_isValid = false;
        }
    } else {
        m_errorString = QString("XMLRPC format error: tag should be either %1 "
                                "or %2, received %3")
                                .arg(tagParams)
                                .arg(tagFault)
                                .arg(tagName);
        m_isValid = false;
    }

    m_isValid = true;
    return res;
}

QString Response::error() const
{
    return m_errorString;
}

QVariant Response::value(const QDomElement &node)
{
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
                return fromArray(data);
            } else {
                return QVariant::Invalid;
            }
        }
    } else {
        m_errorString = QString("XMLRPC format error: unexpected node name "
                                "for value. Expected %1, found %2")
                                .arg(tagValue)
                                .arg(node.nodeName());
        return QVariant::Invalid;
    }
}

QDateTime Response::fromDateTime(const QString &datetime)
{
    QDateTime res = QDateTime::fromString(datetime, Qt::ISODate);
    return res;
}

QVariant Response::fromStruct(const QDomElement &node)
{
    if (node.tagName() != tagStruct) {
        m_errorString = QString("XMLRPC format error: unexpected tag name "
                                "for struct. Expected %1, found %2")
                                .arg(tagStruct)
                                .arg(node.tagName());
        return QVariant::Invalid;
    }

    QMap<QString, QVariant> result;
    QDomElement memberElement = node.firstChild().toElement();
    // Iterate over the <member> tags.
    while (!memberElement.isNull()) {
        QString name;
        QVariant val;
        QDomElement childElement = memberElement.firstChild().toElement();
        while (!childElement.isNull()) {
            QString tag = childElement.tagName();
            if (tag == tagName) {
                name = childElement.text();
            } else if (tag == tagValue) {
                val = value(childElement);
            } else {
                // Unexpected tag name.
                return QVariant::Invalid;
            }

            // Proceed the next element.
            childElement = childElement.nextSibling().toElement();
        }

        if (!name.isNull() && val.isValid()) {
            result[name] = val;
        }

        memberElement = memberElement.nextSibling().toElement();
    }

    return result;
}

QVariant Response::fromArray(const QDomElement& node)
{
    if(node.tagName() != tagArray) {
        m_errorString = QString("XMLRPC format error: unexpected tag name "
                                "for array. Expected %1, found %2")
                                .arg(tagArray)
                                .arg(node.tagName());
        return QVariant::Invalid;
    }

    QVariantList result;
    QDomElement dataElement = node.firstChildElement().firstChildElement();

    while (!dataElement.isNull()) {
        if (dataElement.tagName() == tagValue) {
            result.append(value(dataElement));
        } else {
            // Unexpected tag name.
            return QVariant::Invalid;
        }

        // Process the next element.
        dataElement = dataElement.nextSibling().toElement();
    }

    return result;
}

} // namespace xmlrpc
