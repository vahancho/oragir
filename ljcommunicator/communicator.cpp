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

#include <memory>
#include <QtNetwork>
#include <QCryptographicHash>
#include "communicator.h"
#include "request.h"
#include "response.h"

namespace  lj
{

const char strUserAgent[] = "Oragir v0.2; "
                            "http://sourceforge.net/projects/oragir; "
                            "vahancho@gmail.com";

Communicator::Communicator(QObject *parent)
    :
        QObject(parent)
{
    init();
}

Communicator::~Communicator()
{
    delete m_http;
    qDeleteAll( m_responses );
}

void Communicator::init()
{
    m_http = new QHttp(this);
    m_userAgent = strUserAgent; // Lib version
    setHost("www.livejournal.com", 80);
    setPath("/interface/xmlrpc");
    connect(m_http, SIGNAL(requestFinished(int, bool)), SLOT(requestFinished(int, bool)));
}

void Communicator::setHost(const QString &host, quint16 port)
{
    m_host = host;
    m_http->setHost(host, port);
}

void Communicator::setPath(const QString &path)
{
    m_path = path;
}

void Communicator::setProxy(const QString &host, int port, 
                            const QString &userName, const QString &password)
{
    m_http->setProxy(host, port, userName, password);
}

void Communicator::setUser(const QString &userName, const QString &password)
{
    m_userName = userName;
    m_password = password;
}

void Communicator::setUserAgent(const QString &userAgent)
{
    m_userAgent = userAgent;
}

QByteArray Communicator::getChallenge()
{
    // Send request for the challenge.
    request("LJ.XMLRPC.getchallenge", QVariantList());

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    xmlrpc::Response response(buf);
    QByteArray challenge;

    if (response.isValid()) {
        QVariant responceData = response.data();
        QMap<QString, QVariant> map = responceData.toMap();
        challenge = map.value("challenge").toByteArray();
        int serverTime = map.value("server_time").toInt();
        int expireTime = map.value("expire_time").toInt();
        int challangeLife = expireTime - serverTime;
    }

    return challenge;
}

QVariantList Communicator::authParams()
{
    QVariantList params;
    QByteArray challenge = getChallenge();
    if (challenge.size() == 0)
        return params;

    // Encode user credentials.
    // From LJ docs: "For your response, you then build a MD5 hex digest of the
    // formula (challenge + MD5_hex(password))...".
    QByteArray passMd5 = QCryptographicHash::hash(m_password.toUtf8(),
                                                  QCryptographicHash::Md5).toHex();
    QByteArray authStr = QCryptographicHash::hash(challenge + passMd5,
                                                  QCryptographicHash::Md5).toHex();

    QMap<QString, QVariant> requestStruct;
    requestStruct["username"] = m_userName;
    requestStruct["auth_method"] = "challenge";
    requestStruct["auth_challenge"] = challenge;
    requestStruct["auth_response"] = authStr;

    params.push_back(requestStruct);
    return params;
}

QMap<QString, QVariant> Communicator::login()
{
    QMap<QString, QVariant> result;
    QVariantList params = authParams();
    if (params.size() == 0)
        return result;

    // Send request to login.
    request("LJ.XMLRPC.login", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    xmlrpc::Response response(buf);

    if (response.isValid()) {
        QVariant responceData = response.data();
        result = responceData.toMap();
    }

    return result;
}

void Communicator::getUserTags()
{
    QVariantList params = authParams();
    request("LJ.XMLRPC.getusertags", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();
    qDebug() << buf;

    xmlrpc::Response response(buf);

    if (response.isValid()) {
        QVariant responceData = response.data();
        QMap<QString, QVariant> result = responceData.toMap();
    }
}

void Communicator::request(QString methodName, const QVariantList &params)
{
    QBuffer *responceBuffer = new QBuffer;
    xmlrpc::Request request;
    QByteArray data = request.compose(methodName, params);

    // Generate a header according to XML-RPC spec (http://www.xmlrpc.com/spec)
    QHttpRequestHeader header("POST", m_path);
    header.setValue("host", m_host);
    header.setContentType("text/xml");
    header.setValue("User-Agent", m_userAgent);
    header.setValue("Connection", "Keep-Alive");
    header.setContentLength(data.size());

    // Send the request to server.
    m_currentRequestId = m_http->request(header, data, responceBuffer);
    m_responses[m_currentRequestId] = responceBuffer;
    m_http->close();

    // Block the event loop untill request finished.
    m_eventLoop.exec();
}

void Communicator::requestFinished(int id, bool error)
{
    if (id != m_currentRequestId)
        return;

    m_eventLoop.exit();
}

}
