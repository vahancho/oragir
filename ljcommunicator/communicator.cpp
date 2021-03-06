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
#include "ljsecurity.h"

namespace  lj
{

const char strUserAgent[] = "Oragir LJ Communicator; "
                            "http://sourceforge.net/projects/oragir; "
                            "vahancho@gmail.com";

const char strLJHost[] = "www.livejournal.com";
const char strInterfacePath[] = "/interface/xmlrpc";

// Timeout between two requests to the server.
const int timeout = 2000;

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
    setHost(strLJHost, 80);
    setPath(strInterfacePath);
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

    xmlrpc::Response response;
    QVariant responceData = response.parse(buf);
    QByteArray challenge;

    if (response.isValid()) {
        QMap<QString, QVariant> map = responceData.toMap();
        challenge = map.value("challenge").toByteArray();
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

UserInfo Communicator::login()
{
    QVariantList params = authParams();
    if (params.size() == 0)
        return UserInfo();

    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    param["getpickwurls"] = "1";
    param["getpickws"] = "1";
    param["getmoods"] = "1";
    params.push_back(param);

    // Send request to login.
    request("LJ.XMLRPC.login", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    UserInfo userInfo(buf);
    return userInfo;
}

UserTags Communicator::getUserTags()
{
    QVariantList params = authParams();
    if (params.size() == 0)
        return UserTags();

    request("LJ.XMLRPC.getusertags", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    UserTags tags(buf);
    return tags;
}

QMap<QString, QVariant> Communicator::getComments(int postid)
{
    QMap<QString, QVariant> result;
    QVariantList params = authParams();
    if (params.size() == 0)
        return result;

    // Modify params by adding the item id.
    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    param["ditemid"] = postid;
    params.push_back(param);

    request("LJ.XMLRPC.getcomments", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    xmlrpc::Response response;
    QVariant responceData = response.parse(buf);

    if (response.isValid()) {
        result = responceData.toMap();
    }

    return result;
}

FriendGroups Communicator::getFriendGroups()
{
    QVariantList params = authParams();
    if (params.size() == 0)
        return FriendGroups();

    request("LJ.XMLRPC.getfriendgroups", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    FriendGroups groups(buf);
    return groups;
}

QVariantList Communicator::getDayCount()
{
    QVariantList result;
    QVariantList params = authParams();
    if (params.size() == 0)
        return result;

    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    param["ver"] = 1;
    params.push_back(param);

    request("LJ.XMLRPC.getdaycounts", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    xmlrpc::Response response;
    QVariant responceData = response.parse(buf);

    if (response.isValid()) {
        QMap<QString, QVariant> data = responceData.toMap();
        result = data["daycounts"].toList();
    }

    return result;
}

SyncItems Communicator::syncitems(const QString &lastsync)
{
    QVariantList params = authParams();
    if (params.size() == 0)
        return SyncItems();

    if (!lastsync.isEmpty()) {
        // Modify params by adding the lastsync time.
        QMap<QString, QVariant> param = params.takeAt(0).toMap();
        param["lastsync"] = lastsync;
        params.push_back(param);
    }

    request("LJ.XMLRPC.syncitems", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    SyncItems items(buf);
    return items;
}

Events Communicator::getEvents(const QString &lastsync)
{
    QVariantList params = authParams();
    if (params.size() == 0)
        return Events();

    // Modify params by adding the item id.
    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    // Make sure we use utf-8 encoded strings in response.
    param["ver"] = 1;
    param["prefersubject"] = 0;
    //param["selecttype"] = "lastn";
    //param["howmany"] = 8;
    param["selecttype"] = "syncitems";

    if (!lastsync.isEmpty()) {
        param["lastsync"] = lastsync;
    }
    params.push_back(param);

    request("LJ.XMLRPC.getevents", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    Events events(buf);
    return events;
}

Events Communicator::getEvents(int howmany, const QString &beforedate)
{
    QVariantList params = authParams();
    if (params.size() == 0)
        return Events();

    // Modify params by adding the item id.
    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    // Make sure we use utf-8 encoded strings in response.
    param["ver"] = 1;
    param["prefersubject"] = 0;
    param["selecttype"] = "lastn";
    param["howmany"] = howmany;

    if (!beforedate.isEmpty()) {
        param["beforedate"] = beforedate;
    }
    params.push_back(param);

    request("LJ.XMLRPC.getevents", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    Events events(buf);
    return events;
}

Events Communicator::getEvent(int itemId)
{
    QVariantList params = authParams();
    if (params.size() == 0)
        return Events();

    // Modify params by adding the item id.
    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    // Make sure we use utf-8 encoded strings in response.
    param["ver"] = 1;
    param["prefersubject"] = 0;
    param["selecttype"] = "one";
    param["itemid"] = itemId;

    params.push_back(param);

    request("LJ.XMLRPC.getevents", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    Events events(buf);
    return events;
}

Events Communicator::getDayEvents(const QString &dateStr)
{
    QVariantList params = authParams();
    if (params.size() == 0)
        return Events();

    QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
    if (!date.isValid())
        return Events();

    // Modify params by adding the item id.
    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    // Make sure we use utf-8 encoded strings in response.
    param["ver"] = 1;
    param["prefersubject"] = 0;
    param["selecttype"] = "day";
    param["year"] = date.year();
    param["month"] = date.month();
    param["day"] = date.day();

    params.push_back(param);

    request("LJ.XMLRPC.getevents", params);

    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();

    Events events(buf);
    return events;
}

EventData Communicator::postEvent(const QString &subject, const QString &event,
                                  const Security &security, const QDateTime &dt,
                                  const EventProperties &props,
                                  const QString &journal)
{
    QVariantList params = authParams();
    if (params.size() == 0)
        EventData();

    // Modify params by adding the item id.
    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    // Make sure we use utf-8 encoded strings in response.
    param["ver"] = 1;
    param["subject"] = subject;
    param["event"] = event;
    param["year"] = dt.date().year();
    param["mon"] = dt.date().month();
    param["day"] = dt.date().day();
    param["hour"] = dt.time().hour();
    param["min"] = dt.time().minute();
    param["usejournal"] = journal;
    param["props"] = props.data();

    param["security"] = security.selectedMajorLJName();
    if (security.type() == Security::UseMask) {
        param["allowmask"] = security.mask();
    }

    params.push_back(param);

    request("LJ.XMLRPC.postevent", params);
    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();
    return EventData(buf);
}

EventData Communicator::editEvent(int id, const QString &subject,
                                  const QString &event, const Security &security,
                                  const QDateTime &dt,
                                  const EventProperties &props,
                                  const QString &journal)
{
    QVariantList params = authParams();
    if (params.size() == 0)
        EventData();

    // Modify params by adding the item id.
    QMap<QString, QVariant> param = params.takeAt(0).toMap();
    // Make sure we use utf-8 encoded strings in response.
    param["ver"] = 1;
    param["itemid"] = id;
    param["subject"] = subject;
    param["event"] = event;
    param["year"] = dt.date().year();
    param["mon"] = dt.date().month();
    param["day"] = dt.date().day();
    param["hour"] = dt.time().hour();
    param["min"] = dt.time().minute();
    param["usejournal"] = journal;
    param["props"] = props.data();

    param["security"] = security.selectedMajorLJName();
    if (security.type() == Security::UseMask) {
        param["allowmask"] = security.mask();
    }

    params.push_back(param);

    request("LJ.XMLRPC.editevent", params);
    std::auto_ptr<QBuffer> buffer(m_responses.take(m_currentRequestId));
    QByteArray buf = buffer->buffer();
    return EventData(buf);
}

EventData Communicator::deleteEvent(int id)
{
    return editEvent(id, "", "", Security(), QDateTime(),
                     EventProperties(), "");
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

void Communicator::requestFinished(int id, bool /*error*/)
{
    if (id != m_currentRequestId)
        return;

    // Exit event loop after a timeout. This will prevent sending
    // requests to the server too often.
    QTimer::singleShot(timeout, this, SLOT(exitEventLoop()));
}

void Communicator::exitEventLoop()
{
    m_eventLoop.exit();
}

}
