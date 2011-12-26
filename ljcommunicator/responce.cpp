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

Response::Response()
{}

QVariant Response::data() const
{
    return m_data;
}

bool Response::parse(const QByteArray &responce)
{
    QDomDocument doc;
    int domErrorLine;
    int domErrorColumn;

    if (!doc.setContent(responce, &m_errorString, &domErrorLine, &domErrorColumn)) {
        return false;
    }

    // The body of the response is a single XML structure, a <methodResponse>,
    // which can contain a single <params> which contains a single <param> which
    // contains a single <value>.
    QDomElement methodResponse = doc.firstChildElement("methodResponse");

    if (methodResponse.isNull()) {
        m_errorString = "Missing methodResponse tag";
        return false;
    }

    QDomElement resNode = methodResponse.firstChildElement();
    // The <methodResponse> could also contain a <fault> which contains
    // a <value> which is a <struct> containing two elements, one named
    // <faultCode>, an <int> and one named <faultString>, a <string>.
    // A <methodResponse> can not contain both a <fault> and a <params>.

    QString tagName = resNode.tagName();

    if (tagName == "params") {
        // <params> contains a single <param> which contains a single <value>.
        QDomElement paramValue = resNode.firstChild().firstChild().toElement();

        m_data = QVariant(Converter::fromDomElement(paramValue));

        if (!m_data.isValid()) {
            m_errorString = "Invalid return value";
        }
    } else if (tagName == "fault" ) {
        QDomElement paramValue = resNode.firstChildElement();

        // Should be QMap
        QVariant res = Converter::fromDomElement(paramValue);
        if (res.type() != QVariant::Map) {
            m_errorString = QString("Expected struct for xmlrpc fault, but received %1")
                                    .arg(paramValue.firstChildElement().nodeName());
            return false;
        }

        QMap<QString, QVariant> map = res.toMap();
        if (!map.contains("faultCode") || !map.contains("faultString")) {
            m_errorString = "Fault struct doesn't contain fault code or string values";
            return false;
        }
    } else {
        m_errorString = QString("Unexpected tag: %1").arg(resNode.tagName());
        return false;
    }

    if (m_data.isNull()) {
        return false;
    }

    return true;
}

} // namespace xmlrpc
