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
#include "converter.h"

namespace xmlrpc
{

const char tagParams[] = "params";
const char tagParam[] = "param";
const char tagValue[] = "value";
const char tagFault[] = "fault";
const char tagFaultCode[] = "faultCode";
const char tagFaultString[] = "faultString";

Response::Response(const QByteArray &response)
    :
        m_isValid(false)
{
    m_isValid = parse(response);
}

QVariant Response::data() const
{
    return m_data;
}

bool Response::isValid() const
{
    return m_isValid;
}

bool Response::parse(const QByteArray &response)
{
    QDomDocument doc;
    int domErrorLine;
    int domErrorColumn;

    if (!doc.setContent(response, &m_errorString, &domErrorLine, &domErrorColumn)) {
        return false;
    }

    // From the xmlrpc spec: The body of the response is a single XML structure,
    // a <methodResponse>, which can contain a single <params> which 
    // contains a single <param> which contains a single <value>.
    QDomElement methodResponse = doc.firstChildElement("methodResponse");

    if (methodResponse.isNull()) {
        m_errorString = "XMLRPC format error: missed methodResponse tag";
        return false;
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
            return false;
        }

        QDomElement valueElement = paramElement.firstChild().toElement();
        tagName = valueElement.tagName();
        if (tagName != tagValue) {
            m_errorString = QString("XMLRPC format error: expected %1, found %2")
                                    .arg(tagValue)
                                    .arg(tagName);
            return false;
        }

        // Read param data.
        m_data = Converter::fromDomElement(valueElement);

        if (!m_data.isValid()) {
            m_errorString = "Invalid responce value";
            return false;
        }
    } else if (tagName == tagFault ) {
        QDomElement paramValue = subNode.firstChildElement();

        // Should be QMap
        QVariant res = Converter::fromDomElement(paramValue);
        if (res.type() != QVariant::Map) {
            m_errorString = QString("XMLRPC format error: responce fault "
                                    "value is not a struct");
            return false;
        }

        QMap<QString, QVariant> map = res.toMap();
        if (!map.contains(tagFaultCode) || !map.contains(tagFaultString)) {
            m_errorString = "XMLRPC format error: Fault struct should contain "
                            "either fault code or fault description";
            return false;
        }
    } else {
        m_errorString = QString("XMLRPC format error: tag should be either %1 "
                                "or %2, received %3")
                                .arg(tagParams)
                                .arg(tagFault)
                                .arg(tagName);
        return false;
    }

    if (m_data.isNull()) {
        return false;
    }

    return true;
}

QString Response::error() const
{
    return m_errorString;
}

} // namespace xmlrpc
