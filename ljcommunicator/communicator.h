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

    void setHost(const QString &host, quint16 port = 80);
    void setPath(const QString &path);
    void setProxy(const QString &host, int port, 
                  const QString &username = QString(), const QString &password = QString());
    void setUser(const QString &userName, const QString &password);
    void setUserAgent(const QString &userAgent);

    ~Communicator();

    int request(QString methodName, const QVariantList &params);

signals:
    void done(int requestId, const QVariant &value);
    void failed(int requestId, const QString &errorString);
    void authenticationRequired(const QString &hostname, quint16 port, QAuthenticator *);
    void proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *);

protected slots:
    void requestFinished(int id, bool error);

private:
    void init();

    QString m_host;
    QString m_path;
    QString m_userName;
    QString m_password;
    QString m_userAgent;

    QHttp *m_http;
    QAuthenticator m_proxyAuth;

    QMap<int, QBuffer *> m_responses;
};

} // namespace

#endif // __COMMUNICATOR_H__

