/**************************************************************************
*   Copyright (C) 2012 by Vahan Aghajanyan                                *
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

#ifndef __IMAGEDOWNLOADER_H__
#define __IMAGEDOWNLOADER_H__

#include <QObject>
#include <QPixmap>
#include <QEventLoop>

class QUrl;
class QNetworkReply;
class QNetworkAccessManager;

namespace gui
{

/// Utility class for downloading images from the network.
class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    ImageDownloader(QObject *parent = 0);

    void download(const QString &urlStr);
    void download(const QUrl &url);

    QPixmap pixmap() const;

private slots:
    void onNetManagerFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_netManager;
    QPixmap m_pixmap;

    /// Event loop to make Http requests work synchronously.
    QEventLoop m_eventLoop;
};

} // namespace gui

#endif // __IMAGEDOWNLOADER_H__
