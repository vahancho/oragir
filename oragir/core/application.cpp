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

#include <QStatusBar>
#include <QDesktopServices>
#include "application.h"
#include "defaultManager.h"
#include "versionManager.h"
#include "../parser/atomParser.h"
#include "../database/database.h"
#include "../gui/mainWindow.h"
#include "../strings/strings.h"

namespace core
{

// Default main window size and position
const int defSizeX  = 800;
const int defSizeY  = 700;
const int defPosX   = 200;
const int defPosY   = 200;

// Definition of the pointer to the only application object.
Application *Application::m_theApp = 0;

Application::Application()
    :
        m_defaultManager(0),
        m_mainWindow(0)
{
    // Initialize the application pointer with this instance.
    m_theApp = this;

    // Initialize Application components.
    init();
}

Application::~Application()
{
    // Delete Application components.
    saveMainWindowDefaults();
    m_mainWindow->close();
    delete m_mainWindow;

    m_atomParser->stop();
    delete m_atomParser;

    saveDatabaseDefaults();
    delete m_dataBase;

    delete m_versionManager;

    // Save defaults
    m_defaultManager->saveDefaults();
    delete m_defaultManager;
}

Application *Application::create()
{
    // If application exists - return it, otherwise return the newly created one.
    if(m_theApp)
        return m_theApp;

    return new Application;
}

Application *Application::theApp()
{
    return m_theApp;
}

void Application::destroy()
{
    delete m_theApp;
    m_theApp = 0;
}

void Application::init()
{
    // Set the application name.
    QCoreApplication::setApplicationName(str::AppName);

    // Create all application components
    m_defaultManager = new DefaultManager;

    m_versionManager = new VersionManager;
    m_defaultManager->addProperty(str::CheckUpdates, bool(true), bool(true));

    m_dataBase = new Database;
    registerDatabaseDefaults();

    m_atomParser = new AtomParser;
    m_defaultManager->addProperty(str::Reconnect, bool(true), bool(true));
    m_defaultManager->addProperty(str::ReconnectCount, int(20), int(20));
    QObject::connect(m_atomParser, SIGNAL(fetched(const Post &, const Blog &)),
                     m_dataBase, SLOT(onFetched(const Post &, const Blog &)));

    m_mainWindow = new gui::MainWindow;
    registerMainWindowDefaults();
    QObject::connect(m_dataBase, SIGNAL(recordInserted(const QString &)),
                     m_mainWindow, SLOT(onRecordInserted(const QString &)));

    // Read and set all defaults.
    m_defaultManager->readDefaults();

    restoreDatabase();
    restoreMainWindow();
    m_mainWindow->show();

    // Check for updates if needed.
    if (m_defaultManager->value(str::CheckUpdates).toBool() == true) {
        m_versionManager->checkForUpdates();
    }
}

gui::MainWindow *Application::mainWindow() const
{
    return m_mainWindow;
}

DefaultManager *Application::defaultManager() const
{
    return m_defaultManager;
}

Database *Application::database() const
{
    return m_dataBase;
}

AtomParser *Application::streamParser() const
{
    return m_atomParser;
}

VersionManager *Application::versionManager() const
{
    return m_versionManager;
}

void Application::registerDatabaseDefaults() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_dataBase);

    // Get settings directory and create it if it does not exist.
    QString location =
       QDesktopServices::storageLocation(QDesktopServices::DataLocation) + '/';
    location = QDir::toNativeSeparators(location);
    QDir dir(location);
    if (!dir.exists())
        dir.mkpath(location);

    QString appDataFile = QString(location + "oragir.data");
    m_defaultManager->addProperty(str::Database, appDataFile, appDataFile);

    QString filterFile = QString(location + "filters.flt");
    m_defaultManager->addProperty(str::Filters, filterFile, filterFile);
}

void Application::restoreDatabase() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_dataBase);
    Q_ASSERT(m_mainWindow);

    QString database = m_defaultManager->value(str::Database).toString();
    if (m_dataBase->create(database)) {
        QStringList tables = m_dataBase->tables();
        foreach(const QString &table, tables) {
            m_dataBase->addTable(table);
            m_mainWindow->createFolderView(table);
        }
    }

    QString filtersFile = m_defaultManager->value(str::Filters).toString();
    m_dataBase->openFilters(filtersFile);
}

void Application::saveDatabaseDefaults() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_dataBase);

    m_defaultManager->setValue(str::Database, m_dataBase->databaseName());
    QString filtersFile = m_defaultManager->value(str::Filters).toString();
    if (!filtersFile.isEmpty())
        m_dataBase->saveFilters(filtersFile);
}

void Application::registerMainWindowDefaults() const
{
    Q_ASSERT(m_defaultManager);

    m_defaultManager->addProperty(str::MainWindowSize, QSize(defSizeX, defSizeY), QSize(defSizeX, defSizeY));
    m_defaultManager->addProperty(str::MainWindowPos, QPoint(defPosX, defPosY), QPoint(defPosX, defPosY));
    m_defaultManager->addProperty(str::MainWindowMax, bool(false), bool(false));
    m_defaultManager->addProperty(str::MainWindowState, QByteArray(), QByteArray());
    m_defaultManager->addProperty(str::ShowStatusBar, bool(true), bool(true));
    m_defaultManager->addProperty(str::QuitOnClose, bool(false), bool(false));
}

void Application::restoreMainWindow() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_mainWindow);

    // Set maximized state
    bool max = m_defaultManager->value(str::MainWindowMax).toBool();
    if(max)
        m_mainWindow->showMaximized();

    // Set window size
    QSize size = m_defaultManager->value(str::MainWindowSize).toSize();
    m_mainWindow->resize(size);

    // Set window position
    QPoint pos = m_defaultManager->value(str::MainWindowPos).toPoint();
    m_mainWindow->move(pos);

    // Restore tool bars state
    QByteArray state = m_defaultManager->value(str::MainWindowState).toByteArray();
    m_mainWindow->restoreState(state);

    // Restore status bar state
    bool showStatus = m_defaultManager->value(str::ShowStatusBar).toBool();
    m_mainWindow->setStatusBarVisible(showStatus);
}

void Application::saveMainWindowDefaults() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_mainWindow);

    // Save Main Window configuration to defaults
    m_defaultManager->setValue(str::MainWindowSize, m_mainWindow->size());
    m_defaultManager->setValue(str::MainWindowPos, m_mainWindow->pos());
    m_defaultManager->setValue(str::MainWindowMax, m_mainWindow->isMaximized());
    m_defaultManager->setValue(str::MainWindowState, m_mainWindow->saveState());
    m_defaultManager->setValue(str::ShowStatusBar, m_mainWindow->statusBarVisible());
}

} // namespace core