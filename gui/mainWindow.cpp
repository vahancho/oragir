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

#include <QtGui>
#include "mainWindow.h"
#include "databaseView.h"
#include "../core/application.h"
#include "../core/defaultManager.h"
#include "../parser/atomParser.h"
#include "../database/database.h"
#include "../strings/strings.h"
#include "../strings/guiStrings.h"

namespace gui
{

// Default window size and position
const int defSizeX  = 800;
const int defSizeY  = 700;
const int defPosX   = 200;
const int defPosY   = 200;

// Tool bars icon size
const int iconSizeX = 16;
const int iconSizeY = 16;

// Constructor
MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    :
        QMainWindow(parent, flags),
        m_trayIcon(0),
        m_processedItemCount(0),
        m_recordedItemCount(0)
{
    createMenus();

    m_mdiArea.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // Set MDI area as a central widget
    setCentralWidget(&m_mdiArea);

    // Handle MDI sub windows activation.
    connect(&m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(onSubWindowActivated(QMdiSubWindow *)));

    m_windowMapper = new QSignalMapper(this);

    // Set MDI sub windows mapping.
    connect(m_windowMapper, SIGNAL(mapped(QWidget *)),
            this, SLOT(setActiveSubWindow(QWidget *)));

    // Set window title
    setWindowTitle(str::AppName);

    // Register Main Window defaults
    addDefaults();

    if(QSystemTrayIcon::isSystemTrayAvailable())
        createTrayIcon();

    core::AtomParser *parser = core::Application::theApp()->streamParser();
    connect(parser, SIGNAL(fetched(const Post &, const Blog &)),
            this, SLOT(onItemProcessed()));
    connect(parser, SIGNAL(stateChanged(int)),
            this, SLOT(onParserStateChanged(int)));

    QDockWidget *dock = new QDockWidget("Databases", this);
    dock->setObjectName("Databases");

    m_databaseList = new QTreeWidget;
    m_databaseList->setColumnCount(2);
    m_databaseList->setRootIsDecorated(false);
    QStringList headerLabels;
    headerLabels << "Alias" << "Full Name";
    m_databaseList->setHeaderLabels(headerLabels);
    m_databaseList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_databaseList, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(onDatabaseContextMenu(const QPoint &)));
    connect(m_databaseList, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(onDatabaseItemDblClicked(const QModelIndex &)));

    dock->setWidget(m_databaseList);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

// Destructor
MainWindow::~MainWindow()
{}

void MainWindow::setDatabaseTable(const QSqlDatabase &db, const QString &table)
{
    DatabaseView *dbView = new DatabaseView;
    dbView->init(db, table);

    QMdiSubWindow *postTableView = new QMdiSubWindow;
    postTableView->setWidget(dbView);
    postTableView->setAttribute(Qt::WA_DeleteOnClose);
    postTableView->resize(200, 200);
    postTableView->setWindowTitle("Database: " + db.databaseName() + " " +
                                  " [" +  table + "]");
    m_mdiArea.addSubWindow(postTableView);
    postTableView->showMaximized();
}

void MainWindow::createTrayIcon()
{
    QMenu *trayIconMenu = new QMenu(this);

    QAction *action = new QAction(str::ActionMinimize, this);
    connect(action, SIGNAL(triggered()), this, SLOT(hide()));
    trayIconMenu->addAction(action);

    action = new QAction(str::ActionMaximize, this);
    connect(action, SIGNAL(triggered()), this, SLOT(showMaximized()));
    trayIconMenu->addAction(action);

    action = new QAction(str::ActionRestore, this);
    connect(action, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayIconMenu->addAction(action);

    trayIconMenu->addSeparator();

    action = new QAction(str::ActionQuit, this);
    action->setIcon(QIcon(":icons/exit"));
    connect(action, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayIconMenu->addAction(action);

    m_trayIcon = new QSystemTrayIcon(QIcon(":/icons/app"), this);

    connect(m_trayIcon, SIGNAL(messageClicked()),
            this, SLOT(trayIconMessageClicked()));

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    m_trayIcon->setContextMenu(trayIconMenu);
    m_trayIcon->setToolTip(str::AppName);
    m_trayIcon->setVisible(true);
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:

    if(isMinimized() || !isVisible() || !isActiveWindow()) {
        showNormal();
        activateWindow();
        raise();
    }

    break;

    case QSystemTrayIcon::MiddleClick:

        break;
    default:
        ;
    }
}

void MainWindow::trayIconMessageClicked()
{}

void MainWindow::onAbout()
{
    QMessageBox::about(this, str::AboutTitle,
                       QString(str::About).arg(str::Version));
}

void MainWindow::createDockWindows()
{}

void MainWindow::createMenus()
{
    //////////////////////////////////////////////////////////////////////////
    // File menu
    //
    QMenu *fileMenu = new QMenu(str::MenuFile, this);
    QToolBar *fileToolBar = new QToolBar(str::MenuFile, this);
    fileToolBar->setObjectName(str::MenuFile);
    fileToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    QMenu *newMenu = fileMenu->addMenu(str::MenuNew);

    QAction *openAction = fileMenu->addAction(str::ActionOpen);
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));
    connect(openAction, SIGNAL(triggered()), this, SLOT(onDatabaseOpen()));
    fileToolBar->addAction(openAction);

    fileMenu->addSeparator();
    QAction *quitAction = fileMenu->addAction(str::ActionExit);
    quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    quitAction->setIcon(QIcon(":icons/exit"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    //////////////////////////////////////////////////////////////////////////
    // View menu
    //
    QMenu *viewMenu = new QMenu(str::MenuView, this);

    //QToolBar *viewToolBar = new QToolBar(str::MenuView, this);
    //viewToolBar->setObjectName(str::MenuView);
    //viewToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    // Add 'Views' sub menu
    m_viewsMenu = viewMenu->addMenu(str::MenuViews);
    connect(m_viewsMenu, SIGNAL(aboutToShow()), this, SLOT(updateViewsMenu()));

    // Add 'Toolbars' sub menu
    m_toolBarsMenu = viewMenu->addMenu(str::MenuToolBars);
    connect(m_toolBarsMenu, SIGNAL(aboutToShow()), this, SLOT(updateToolBarsMenu()));

    // Add StatusBar action
    m_statusBarAction = viewMenu->addAction(str::ActionStatusBar);
    m_statusBarAction->setCheckable(true);

    // Set connection
    connect(m_statusBarAction, SIGNAL(triggered()), this, SLOT(onStatusBarShowHide()));

    //////////////////////////////////////////////////////////////////////////
    // Stream menu
    //
    QMenu *streamMenu = new QMenu(str::MenuStream, this);
    QToolBar *streamToolBar = new QToolBar(str::MenuStream, this);
    streamToolBar->setObjectName(str::MenuStream);
    streamToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    QAction *startAction = streamMenu->addAction(str::ActionStart);
    connect(startAction, SIGNAL(triggered()), this, SLOT(onStreamStart()));
    streamToolBar->addAction(startAction);

    QAction *stopAction = streamMenu->addAction(str::ActionStop);
    connect(stopAction, SIGNAL(triggered()), this, SLOT(onStreamStop()));
    streamToolBar->addAction(stopAction);

    //////////////////////////////////////////////////////////////////////////
    // Tools menu
    //
    QMenu *toolsMenu = new QMenu(str::MenuTools, this);
    QToolBar *toolsToolBar = new QToolBar(str::MenuTools, this);
    toolsToolBar->setObjectName(str::MenuTools);
    toolsToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    QAction *filtersAction = toolsMenu->addAction(str::ActionFilters);
    connect(filtersAction, SIGNAL(triggered()), this, SLOT(onFilters()));
    toolsToolBar->addAction(filtersAction);

    QAction *optionsAction = toolsMenu->addAction(str::ActionOptions);
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(onOptions()));
    toolsToolBar->addAction(optionsAction);

    //////////////////////////////////////////////////////////////////////////
    // Window menu
    m_windowMenu = new QMenu(str::MenuWindow, this);
    connect(m_windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    m_closeAct = new QAction(str::ActionClose, this);
    m_closeAct->setShortcut(tr("Ctrl+F4"));
    //m_closeAct->setStatusTip(str::MsgCloseActiveWnd);
    connect(m_closeAct, SIGNAL(triggered()), &m_mdiArea, SLOT(closeActiveSubWindow()));

    m_closeAllAct = new QAction(str::ActionCloseAll, this);
    //m_closeAllAct->setStatusTip(str::MsgCloseAllWnd);
    connect(m_closeAllAct, SIGNAL(triggered()),	&m_mdiArea, SLOT(closeAllSubWindows()));

    m_tileAct = new QAction(str::ActionTile, this);
    //m_tileAct->setStatusTip(str::MsgTile);
    m_tileAct->setIcon(QIcon(":icons/tile"));
    connect(m_tileAct, SIGNAL(triggered()), &m_mdiArea, SLOT(tileSubWindows()));

    m_cascadeAct = new QAction(str::ActionCascade, this);
    //m_cascadeAct->setStatusTip(str::MsgCascade);
    m_cascadeAct->setIcon(QIcon(":icons/cascade"));
    connect(m_cascadeAct, SIGNAL(triggered()), &m_mdiArea, SLOT(cascadeSubWindows()));

    m_nextAct = new QAction(str::ActionNext, this);
    //m_nextAct->setStatusTip(str::MsgNext);
    connect(m_nextAct, SIGNAL(triggered()), &m_mdiArea, SLOT(activateNextSubWindow()));

    m_previousAct = new QAction(str::ActionPrevious, this);
    //m_previousAct->setStatusTip(str::MsgPrevious);
    connect(m_previousAct, SIGNAL(triggered()),	&m_mdiArea, SLOT(activatePreviousSubWindow()));

    m_separatorAct = new QAction(this);
    m_separatorAct->setSeparator(true);

    //////////////////////////////////////////////////////////////////////////
    // Help menu
    QMenu *helpMenu = new QMenu(str::MenuHelp, this);
    QAction *aboutAction = helpMenu->addAction(str::ActionAbout);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(onAbout()));

    //////////////////////////////////////////////////////////////////////////

    // Add menus to menu bar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(streamMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(m_windowMenu);
    menuBar()->addSeparator();
    menuBar()->addMenu(helpMenu);

    // Add tool bars
    addToolBar(fileToolBar);
    addToolBar(streamToolBar);
    addToolBar(toolsToolBar);
}

void MainWindow::updateToolBarsMenu()
{
    Q_ASSERT(m_toolBarsMenu);

    // Clear menu
    m_toolBarsMenu->clear();

    // Get the list of all tool bars
    QList<QToolBar *> toolbars = qFindChildren<QToolBar *>(this);

    if (toolbars.size() != 0) {
        // If the list is not empty

        // Iterate through all tool bars
        for (int i = 0; i < toolbars.size(); ++i) {
            if (toolbars.at(i)->parentWidget() == this)
                // If the parent of tool bar is main window add toggled action to menu
                m_toolBarsMenu->addAction(toolbars.at(i)->toggleViewAction());
        }
    }
}

void MainWindow::onStatusBarShowHide()
{
    if (statusBar()->isVisible())
        // If status bar visible hide status bar
        statusBar()->setVisible(false);
    else
        statusBar()->setVisible(true);
}

void MainWindow::onToolsOptions()
{}

void MainWindow::saveWindow() const
{
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();

    // Save Main Window configuration to defaults
    defaultMngr->setValue(str::MainWindowSize, size());
    defaultMngr->setValue(str::MainWindowPos, pos());
    defaultMngr->setValue(str::MainWindowMax, isMaximized());
    defaultMngr->setValue(str::MainWindowState, saveState());
    defaultMngr->setValue(str::ShowStatusBar, m_statusBarAction->isChecked());
}

void MainWindow::restoreWindow()
{
    // Read Main Window configuration from defaults and restore the state
    core::DefaultManager *defaultMngr =
                    core::Application::theApp()->defaultManager();

    // Set maximized state
    bool max = defaultMngr->value(str::MainWindowMax).toBool();
    if(max)
        showMaximized();

    // Set window size
    QSize size = defaultMngr->value(str::MainWindowSize).toSize();
    resize(size);

    // Set window position
    QPoint pos = defaultMngr->value(str::MainWindowPos).toPoint();
    move(pos);

    // Restore tool bars state
    QByteArray state = defaultMngr->value(str::MainWindowState).toByteArray();
    restoreState(state);

    // Restore status bar state
    bool showStatus = defaultMngr->value(str::ShowStatusBar).toBool();
    statusBar()->setVisible(showStatus);
    m_statusBarAction->setChecked(showStatus);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    core::DefaultManager *defaultMngr = 
                            core::Application::theApp()->defaultManager();

    bool quit = defaultMngr->value(str::QuitOnClose).toBool();

    if(!quit) {
        hide();
        event->ignore();
    } else
        event->accept();

    saveWindow();
}

void MainWindow::addDefaults() const
{
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();

    // Add Main Window related defaults with initial and factory values.
    defaultMngr->addProperty(str::MainWindowSize, QSize(defSizeX, defSizeY), QSize(defSizeX, defSizeY));
    defaultMngr->addProperty(str::MainWindowPos, QPoint(defPosX, defPosY), QPoint(defPosX, defPosY));
    defaultMngr->addProperty(str::MainWindowMax, bool(false), bool(false));
    defaultMngr->addProperty(str::MainWindowState, QByteArray(), QByteArray());
    defaultMngr->addProperty(str::ShowStatusBar, bool(true), bool(true));
    defaultMngr->addProperty(str::QuitOnClose, bool(false), bool(false));
}

void MainWindow::updateWindowMenu()
{
    // Clear menu actions to add new ones
    m_windowMenu->clear();

    QList<QMdiSubWindow *> windows = m_mdiArea.subWindowList();

    m_windowMenu->addAction(m_closeAct);
    m_closeAct->setEnabled(!windows.isEmpty());
    m_windowMenu->addAction(m_closeAllAct);
    m_closeAllAct->setEnabled(!windows.isEmpty());
    m_windowMenu->addSeparator();

    m_windowMenu->addAction(m_tileAct);
    m_tileAct->setEnabled(!windows.isEmpty());
    m_windowMenu->addAction(m_cascadeAct);
    m_cascadeAct->setEnabled(!windows.isEmpty());
    m_windowMenu->addSeparator();

    m_windowMenu->addAction(m_nextAct);
    m_nextAct->setEnabled(!windows.isEmpty());
    m_windowMenu->addAction(m_previousAct);
    m_previousAct->setEnabled(!windows.isEmpty());
    m_windowMenu->addAction(m_separatorAct);

    m_separatorAct->setVisible(!windows.isEmpty());

    // Add actions corresponding to MDI sub windows
    for (int i = 0; i < windows.size(); ++i) {
        QMdiSubWindow *child = windows.at(i);

        QString text;

        if (i < 9)
            text = tr("&%1 %2").arg(i + 1).arg(child->windowTitle());
        else
            text = tr("%1 %2").arg(i + 1).arg(child->windowTitle());

        QAction *action  = m_windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeSubWindow());
        connect(action, SIGNAL(triggered()), m_windowMapper, SLOT(map()));
        m_windowMapper->setMapping(action, child);
    }
}

void MainWindow::updateViewsMenu()
{
    Q_ASSERT(m_viewsMenu);

    // Clear menu
    m_viewsMenu->clear();

    // Get the list of all tool bars
    QList<QDockWidget *> dockWidgets = qFindChildren<QDockWidget *>(this);

    if (dockWidgets.size() != 0) {
        // If the list is not empty
        // Iterate through all dock widgets
        for (int i = 0; i < dockWidgets.size(); ++i) {
            if (dockWidgets.at(i)->parentWidget() == this)
                // If the parent of dock widget is main window add toggled action to menu
                m_viewsMenu->addAction(dockWidgets.at(i)->toggleViewAction());
        }
    }
}

QMdiSubWindow *MainWindow::activeSubWindow() const
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea.activeSubWindow())
        return activeSubWindow;

    return 0;
}

void MainWindow::onSubWindowActivated(QMdiSubWindow *subWindow)
{}

void MainWindow::setActiveSubWindow(QWidget *subWindow)
{
    if (!subWindow)
        return;

    m_mdiArea.setActiveSubWindow(qobject_cast<QMdiSubWindow *>(subWindow));
}

void MainWindow::onRecordInserted(const QSqlDatabase &db, const QString &table)
{
    // Find database view that has to be updated.
    QList<QMdiSubWindow *> mdiWindows = m_mdiArea.subWindowList();
    foreach(QMdiSubWindow *mdiWindow, mdiWindows) {
        if (DatabaseView *dbView =
            qobject_cast<DatabaseView *>(mdiWindow->widget())){
            if (dbView->hasTable(db, table)) {
                dbView->updateTable();
                break;
            }
        }
    }
}

void MainWindow::onStreamStart()
{
    core::AtomParser *streamParser =
                    core::Application::theApp()->streamParser();
    streamParser->start();
}

void MainWindow::onStreamStop()
{
    core::AtomParser *streamParser =
                    core::Application::theApp()->streamParser();
    streamParser->stop();
}

void MainWindow::onDatabaseOpen()
{
    QFileDialog dlg(this, "Open Database File", ".",
                    tr("Databases (*.db);;All files (*.*)"));
    if(dlg.exec() == QDialog::Accepted) {
        QStringList files = dlg.selectedFiles();
        if (files.size() > 0) {
            core::Database *db = core::Application::theApp()->database();
            QString file = files.at(0);
            if (db->create(file)) {
                setDatabaseTable(db->database(file), "post");

                QTreeWidgetItem *node = new QTreeWidgetItem;
                if (db->isActive(file))
                    node->setIcon(0, QIcon(":/icons/db_active"));
                else
                    node->setIcon(0, QIcon(":/icons/db"));
                QFileInfo fi(file);
                node->setText(0, fi.fileName());
                node->setToolTip(0, fi.fileName());
                node->setText(1, file);
                node->setToolTip(1, file);
                m_databaseList->addTopLevelItem(node);
            } else {
                QMessageBox::critical(this, str::DatabaseError,
                                      db->errorMessage());
            }
        }
    }
}

void MainWindow::onItemProcessed()
{
    m_processedItemCount++;
    QString msg = QString("%1 stream items processed").arg(m_processedItemCount);
    statusBar()->showMessage(msg);
}

void MainWindow::onFilters()
{
}

void MainWindow::onOptions()
{
}

void MainWindow::onDatabaseContextMenu(const QPoint &pos)
{
    if(QTreeWidgetItem *treeItem = m_databaseList->itemAt(pos)) {
        QMenu menu;
        core::Database *db = core::Application::theApp()->database();
        QString dbName = treeItem->text(1);
        bool active = db->isActive(dbName);
        if (!active) {
            QAction *action = menu.addAction(QIcon(":/icons/db_activate"),
                                             "Active",
                                             this,
                                             SLOT(onDatabaseActivate(bool)));
            action->setData(dbName);
            action->setCheckable(true);
            action->setChecked(active);
        }

        QAction *action = menu.addAction(QIcon(":/icons/db_remove"),
                                         "&Remove",
                                         this,
                                         SLOT(onDatabaseRemove()));
        action->setData(dbName);

        menu.exec(m_databaseList->mapToGlobal(QPoint(pos.x(), pos.y() + 20)));
    }
}

void MainWindow::onDatabaseActivate(bool /*activate*/)
{
    if(QAction *action = qobject_cast<QAction *>(sender())) {
        core::Database *db = core::Application::theApp()->database();
        QString dbName = action->data().toString();
        db->setActive(dbName);

        // Set the active icon for the active db item.
        for(int i = 0; i < m_databaseList->topLevelItemCount(); i++) {
            QTreeWidgetItem *item = m_databaseList->topLevelItem(i);
            if (item->text(1) == dbName) {
                item->setIcon(0, QIcon(":/icons/db_active"));
            } else {
                item->setIcon(0, QIcon(":/icons/db"));
            }
        }
    }
}

void MainWindow::onDatabaseRemove()
{
     if(QAction *action = qobject_cast<QAction *>(sender())) {
        core::Database *db = core::Application::theApp()->database();
        QString dbName = action->data().toString();

        // Stop parsing before removing the target database.
        if (db->isActive(dbName)) {
            core::AtomParser *parser =
                    core::Application::theApp()->streamParser();
            parser->stop();
        }

        // Find database view(s) that has to be closed.
        QList<QMdiSubWindow *> mdiWindows = m_mdiArea.subWindowList();
        foreach(QMdiSubWindow *mdiWindow, mdiWindows) {
            if (DatabaseView *dbView =
                qobject_cast<DatabaseView *>(mdiWindow->widget())){
                if (dbView->hasTable(db->database(dbName), "post")) {
                    delete dbView;
                    mdiWindow->close();
                }
            }
        }

        // Remove all nodes for the given database.
        // Start from bottom to top to prevent shifting the indexes.
        for(int i = m_databaseList->topLevelItemCount() - 1; i >= 0 ; --i) {
            QTreeWidgetItem *item = m_databaseList->topLevelItem(i);
            if (item->text(1) == dbName)
                m_databaseList->takeTopLevelItem(i);
        }

        // Finally remove the database itself.
        db->remove(dbName);
    }
}

void MainWindow::onParserStateChanged(int /*state*/)
{
    core::AtomParser *parser = core::Application::theApp()->streamParser();
    statusBar()->showMessage(parser->statusMessage(), 2000);
}

void MainWindow::onDatabaseItemDblClicked(const QModelIndex &index)
{
    core::Database *db = core::Application::theApp()->database();
    QTreeWidgetItem *item = m_databaseList->topLevelItem(index.row());
    QString dbName = item->text(1);
    QList<QMdiSubWindow *> mdiWindows = m_mdiArea.subWindowList();
    foreach(QMdiSubWindow *mdiWindow, mdiWindows) {
        if (DatabaseView *dbView =
            qobject_cast<DatabaseView *>(mdiWindow->widget())){
            if (dbView->hasTable(db->database(dbName), "post")) {
                setActiveSubWindow(mdiWindow);
                break;
            }
        }
    }

}

} // namespace gui
