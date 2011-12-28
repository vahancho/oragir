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

#ifndef __COMMUNICATOR_H__
#define __COMMUNICATOR_H__

#include <QAuthenticator>
#include <QHttp>
#include <QEventLoop>
#include "export.h"

class QBuffer;

namespace lj
{

/// Implements LiveJournal wrapper around of xmlrpc client.
class LJCOMMUNICATOR_EXPORT Communicator : public QObject
{
    Q_OBJECT
public:
    Communicator(QObject *parent = 0);
    ~Communicator();

    void setHost(const QString &host, quint16 port = 80);
    void setPath(const QString &path);
    void setProxy(const QString &host, int port, 
                  const QString &username = QString(), const QString &password = QString());
    void setUser(const QString &userName, const QString &password);
    void setUserAgent(const QString &userAgent);

    /// Implements LJ getchellange command.
    QByteArray getChallenge();
    QMap<QString, QVariant> getUserTags();
    QMap<QString, QVariant> login();
    QMap<QString, QVariant> getComments(int postid);

protected slots:
    void requestFinished(int id, bool error);

private:
    void init();
    void request(QString methodName, const QVariantList &params);

    /// Create and returns authentication parameters list.
    /*!
        This is is a challenge based authentication parameters
        that can be used in query requests to LJ server, such as
        login, getusertags etc.
    */
    QVariantList authParams();

    QString m_host;
    QString m_path;
    QString m_userName;
    QString m_password;
    QString m_userAgent;

    QHttp *m_http;
    QMap<int, QBuffer *> m_responses;

    /// Stores the current Http request id.
    int m_currentRequestId;

    /// Event loop to make Http requests work synchronously.
    QEventLoop m_eventLoop;
};

} // namespace

#endif // __COMMUNICATOR_H__

