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

#include <QtGui/QApplication>
#include <QIcon>
#include "../core/application.h"
#include "../parser/filter.h"
#include "../database/database.h"
#include "../gui/mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    a.setWindowIcon(QIcon(":/icons/app"));

    core::Application *app = core::Application::create();
    core::Database *db = app->database();
    QString appPath = QCoreApplication::applicationDirPath();
    if (!db->create(appPath + "/posts.db")) {
        printf("%s \n", db->errorMessage().toAscii().data());
        return -1;
    }

    if (!db->openFilters(appPath + "/filters.xml")) {
        core::Filter<core::Post> filter("Test filter");
        filter.setRule(str::TagContent, "test", core::Filter<core::Post>::Contains);
        db->addFilter(filter);
    }

    gui::MainWindow *mw = app->mainWindow();
    mw->setDatabaseTable(db->database(), "post");

    int ret = a.exec();
    core::Application::destroy();

    return ret;
}
