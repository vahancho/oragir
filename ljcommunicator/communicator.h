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
#include "ljevents.h"
#include "ljuserinfo.h"
#include "ljusertags.h"
#include "ljsyncitems.h"
#include "ljeventdata.h"
#include "ljfriendgroups.h"

class QBuffer;

namespace lj
{

class Security;

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
    UserTags getUserTags();
    UserInfo login();
    QMap<QString, QVariant> getComments(int postid);

    FriendGroups getFriendGroups();

    /// Returns the list of date-count pairs.
    QVariantList getDayCount();

    /// Returns a list (or part of a list) of all the items.
    /*!
        Returns a list (or part of a list) of all the items (journal entries,
        to-do items, comments) that have been created or updated on LiveJournal
        since you last downloaded them. Note that the items themselves are not
        returned --- only the item type and the item number. After you get this
        you have to go fetch the items using another protocol mode. For journal
        entries (type "L"), use the getevents mode with a selecttype of "syncitems".

        @param lastsync The date you last downloaded synced, in "yyyy-mm-dd hh:mm:ss"
                        format. Note: do not make this date up from the client's 
                        local computer... send the date from this mode's response 
                        of the newest item you have saved on disk.
    */
    SyncItems syncitems(const QString &lastsync = QString());

    /// Returns the item by the given unique item id.
    Events getEvent(int itemId);

    /// Returns the all events after the last synced time.
    /*!
        The date (in "yyyy-mm-dd hh:mm:ss" format) that you
        want to get updates since.
    */
    Events getEvents(const QString &lastsync = QString());

    /// Returns the last events limited by the number.
    /*!
        According to the LJ docs, the max number of events
        that can be returned is limited to 50.
        Use datebefore to restrict all entries returned to be before
        the date you specify, which must be of the form yyyy-mm-dd hh:mm:ss.
    */
    Events getEvents(int howmany, const QString &beforedate = QString());

    /// Returns events for the given day.
    Events getDayEvents(const QString &dateStr);

    EventData postEvent(const QString &subject, const QString &event,
                        const Security &security, const QDateTime &dt,
                        const EventProperties &props,
                        const QString &journal);

    /// Edits existing event given by id. If event is empty string it deleted.
    EventData editEvent(int id, const QString &subject, const QString &event,
                        const Security &security, const QDateTime &dt,
                        const EventProperties &props,
                        const QString &journal);

    EventData deleteEvent(int id);

private slots:
    void requestFinished(int id, bool error);
    void exitEventLoop();

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

