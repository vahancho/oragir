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
#include "dlgFilters.h"
#include "dlgOptions.h"
#include "generalOptionsPage.h"
#include "../core/application.h"
#include "../core/defaultManager.h"
#include "../parser/atomParser.h"
#include "../database/database.h"
#include "../strings/strings.h"
#include "../strings/guiStrings.h"

namespace gui
{

// Tool bars icon size
const int iconSizeX = 16;
const int iconSizeY = 16;

// Constructor
MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    :
        QMainWindow(parent, flags),
        m_trayIcon(0),
        m_processedItemCount(0),
        m_recordedItemCount(0),
        m_statusBarVisible(true)
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

    if(QSystemTrayIcon::isSystemTrayAvailable())
        createTrayIcon();

    core::AtomParser *parser = core::Application::theApp()->streamParser();
    connect(parser, SIGNAL(fetched(const Post &, const Blog &)),
            this, SLOT(onItemProcessed()));
    connect(parser, SIGNAL(stateChanged(int)),
            this, SLOT(onParserStateChanged(int)));
    connect(parser, SIGNAL(dataReadProgress(int, int)),
            this, SLOT(onDataReadProgress(int, int)));

    QDockWidget *dock = new QDockWidget("Folders", this);
    dock->setObjectName("Folders");

    m_foldersList = new QTreeWidget(this);
    m_foldersList->setColumnCount(1);
    m_foldersList->setRootIsDecorated(false);
    QStringList headerLabels;
    headerLabels << "Name";
    m_foldersList->setHeaderLabels(headerLabels);
    m_foldersList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_foldersList, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(onFolderContextMenu(const QPoint &)));
    connect(m_foldersList, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(onFolderDblClicked(const QModelIndex &)));

    dock->setWidget(m_foldersList);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    QLabel *statusLabel = new QLabel(this);
    m_progress = new QMovie(":/icons/progress");
    statusLabel->setMovie(m_progress);
    statusBar()->addPermanentWidget(statusLabel);

    m_processedItems = new QLabel(this);
    m_processedItems->setText("  Pr 0  ");
    m_processedItems->setToolTip("Number of processed items");
    statusBar()->addPermanentWidget(m_processedItems);

    m_processedData = new QLabel(this);
    m_processedData->setText(" Size 0 kB  ");
    m_processedData->setToolTip("Data downloaded (kB)");
    statusBar()->addPermanentWidget(m_processedData);

    m_unreadItems = new QLabel(this);
    m_unreadItems->setText("  Unread: 0  ");
    m_unreadItems->setToolTip("Number of unread items");
    statusBar()->addPermanentWidget(m_unreadItems);

    m_totalItems = new QLabel(this);
    m_totalItems->setText("  Total: 0  ");
    m_totalItems->setToolTip("Total number of items");
    statusBar()->addPermanentWidget(m_totalItems);
}

// Destructor
MainWindow::~MainWindow()
{}

void MainWindow::createFolderView(const QString &table)
{
    core::Database *dbObj = core::Application::theApp()->database();
    QSqlDatabase db = dbObj->database();
    if (!db.isValid())
        return;

    // Duplicate nodes disallowed in database list view, so search
    // among existing nodes and if a node with the given name exists
    // do not add another one.
    QList<QTreeWidgetItem *> nodes =
                m_foldersList->findItems(table, Qt::MatchFixedString, Name);
    if (nodes.size() == 0) {
        // Add tree node for the given database.
        QTreeWidgetItem *node = new QTreeWidgetItem;
        node->setIcon(Name, QIcon(":/icons/folder"));
        node->setText(Name, table);
        node->setToolTip(Name, table);
        m_foldersList->addTopLevelItem(node);
    }

    // Create and show database view.
    DatabaseView *dbView = new DatabaseView(db, table);
    QMdiSubWindow *postTableView = new QMdiSubWindow;
    postTableView->setWidget(dbView);
    postTableView->setAttribute(Qt::WA_DeleteOnClose);
    postTableView->resize(200, 200);
    postTableView->setWindowTitle(table);
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

    QAction *newFolderAction = newMenu->addAction(str::ActionNewFolder);
    newFolderAction->setShortcut(QKeySequence(tr("Ctrl+N")));
    newFolderAction->setIcon(QIcon(":icons/table"));
    connect(newFolderAction, SIGNAL(triggered()), this, SLOT(onNewFolder()));
    fileToolBar->addAction(newFolderAction);

    fileMenu->addSeparator();
    QAction *quitAction = fileMenu->addAction(str::ActionExit);
    quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    quitAction->setIcon(QIcon(":icons/exit"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    //////////////////////////////////////////////////////////////////////////
    // View menu
    //
    QMenu *viewMenu = new QMenu(str::MenuView, this);
    connect(viewMenu, SIGNAL(aboutToShow()), this, SLOT(setStatusBarActionState()));

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
    connect(m_statusBarAction, SIGNAL(toggled(bool)), this, SLOT(setStatusBarVisible(bool)));

    //////////////////////////////////////////////////////////////////////////
    // Stream menu
    //
    QMenu *streamMenu = new QMenu(str::MenuStream, this);
    QToolBar *streamToolBar = new QToolBar(str::MenuStream, this);
    streamToolBar->setObjectName(str::MenuStream);
    streamToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    m_startAction = streamMenu->addAction(str::ActionStart);
    connect(m_startAction, SIGNAL(triggered()), this, SLOT(onStreamStart()));
    m_startAction->setIcon(QIcon(":icons/start"));
    streamToolBar->addAction(m_startAction);

    m_stopAction = streamMenu->addAction(str::ActionStop);
    connect(m_stopAction, SIGNAL(triggered()), this, SLOT(onStreamStop()));
    m_stopAction->setIcon(QIcon(":icons/stop"));
    m_stopAction->setEnabled(false);
    streamToolBar->addAction(m_stopAction);

    //////////////////////////////////////////////////////////////////////////
    // Tools menu
    //
    QMenu *toolsMenu = new QMenu(str::MenuTools, this);
    QToolBar *toolsToolBar = new QToolBar(str::MenuTools, this);
    toolsToolBar->setObjectName(str::MenuTools);
    toolsToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    QAction *filtersAction = toolsMenu->addAction(str::ActionFilters);
    connect(filtersAction, SIGNAL(triggered()), this, SLOT(onFilters()));
    filtersAction->setIcon(QIcon(":icons/filter"));
    toolsToolBar->addAction(filtersAction);

    QAction *fltExportAction = toolsMenu->addAction(str::ActionFilterExport);
    connect(fltExportAction, SIGNAL(triggered()), this, SLOT(onFiltersExport()));
    fltExportAction->setIcon(QIcon(":icons/filter_export"));
    toolsToolBar->addAction(fltExportAction);

    QAction *optionsAction = toolsMenu->addAction(str::ActionOptions);
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(onOptions()));
    optionsAction->setIcon(QIcon(":icons/options"));
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

void MainWindow::setStatusBarVisible(bool visible)
{
    statusBar()->setVisible(visible);
    m_statusBarVisible = visible;
}

bool MainWindow::statusBarVisible() const
{
    return m_statusBarVisible;
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

void MainWindow::setStatusBarActionState()
{
    m_statusBarAction->setChecked(statusBar()->isVisible());
}

QMdiSubWindow *MainWindow::activeSubWindow() const
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea.activeSubWindow())
        return activeSubWindow;

    return 0;
}

void MainWindow::onSubWindowActivated(QMdiSubWindow *subWindow)
{
    if (!subWindow)
        return;

    if (DatabaseView *dbView = qobject_cast<DatabaseView *>(subWindow->widget())) {
        updateStatusLabels(dbView->table());
    }
}

void MainWindow::updateStatusLabels(const QString &table)
{
    core::Database *db = core::Application::theApp()->database();
    m_unreadItems->setText(QString("  Unread: %1  ").arg(db->unreadCount(table)));
    m_totalItems->setText(QString("  Total: %1  ").arg(db->totalCount(table)));
}

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

                // Update statistics for the active mdi window table.
                if (mdiWindow == m_mdiArea.activeSubWindow())
                    updateStatusLabels(table);
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

    // Disable start action until streaming is stopped.
    m_startAction->setEnabled(false);
    m_stopAction->setEnabled(true);
    m_progress->start();
    // Reset the counter.
    m_processedItemCount = 0;
}

void MainWindow::onStreamStop()
{
    core::AtomParser *streamParser =
                    core::Application::theApp()->streamParser();
    streamParser->stop();

    // Enable start action.
    m_startAction->setEnabled(true);
    m_stopAction->setEnabled(false);
    m_progress->stop();
}

void MainWindow::onNewFolder()
{
    bool ok;
    QString table = QInputDialog::getText(this, tr("Create New Folder"),
                                          tr("Folder name:"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);
    if (ok && !table.isEmpty()) {
        core::Database *db = core::Application::theApp()->database();
        if (db->addTable(table)) {
            createFolderView(table);
        } else {
            QMessageBox::critical(this, str::DatabaseError,
                                  db->errorMessage());
        }
    }
}

void MainWindow::onItemProcessed()
{
    m_processedItemCount++;
    QString msg = QString("  Pr %1  ").arg(m_processedItemCount);
    m_processedItems->setText(msg);
}

void MainWindow::onDataReadProgress(int done, int /*total*/)
{
    QString msg;
    if (done < 1048576) {
        msg = QString("  Size %1 kB  ").arg(done / 1024);
    } else {
        double mb = (double)done / 1048576;
        msg = QString("  Size %1 mB  ").arg(mb, 0, 'g', 3);
    }
    m_processedData->setText(msg);
}

void MainWindow::onFilters()
{
    core::Database *db = core::Application::theApp()->database();
    FiltersDialog dlg;
    const core::Database::Filters &filters = db->filters();
    dlg.setFilters(filters);
    if (dlg.exec() == QDialog::Accepted) {
        // Read the filters from dialog and update database.
        const core::Database::Filters &tmpFilters = dlg.filters();
        core::Database::Filters::const_iterator it = tmpFilters.begin();
        // Clear old filters before adding new ones.
        db->clearFilters();
        while (it != tmpFilters.end()) {
            const core::Filter<core::Post> &filter = *it;
            db->addFilter(filter);
            ++it;
        }
    }
}

void MainWindow::onFiltersExport()
{
    QString fileName =
        QFileDialog::getSaveFileName(this, "Save Filter File",
                                     ".",
                                     tr("Oragir Filters (*.flt)"));
    if(!fileName.isEmpty()) {
        core::Database *db = core::Application::theApp()->database();
        db->saveFilters(fileName);
    }
}

void MainWindow::onOptions()
{
    // Create and show dialog.
    OptionsDialog dlg(this);
    dlg.addPage(new gui::GeneralOptionsPage);
    // If dialog accepted, i.e. Ok button pressed, save all defaults.
    if(dlg.exec() == QDialog::Accepted)
        dlg.saveDefaults();
}

void MainWindow::onFolderContextMenu(const QPoint &pos)
{
    if(QTreeWidgetItem *treeItem = m_foldersList->itemAt(pos)) {
        QMenu menu;
        QAction *action = menu.addAction(QIcon(":/icons/folder_delete"),
                                         str::ActionDelete,
                                         this,
                                         SLOT(onFolderDelete()));
        QString folderName = treeItem->text(Name);
        action->setData(folderName);

        menu.exec(m_foldersList->mapToGlobal(QPoint(pos.x(), pos.y() + 20)));
    }
}

void MainWindow::onFolderDelete()
{
     if(QAction *action = qobject_cast<QAction *>(sender())) {
        core::Database *db = core::Application::theApp()->database();
        QString folderName = action->data().toString();

        // Find database view(s) that has to be closed.
        QList<QMdiSubWindow *> mdiWindows = m_mdiArea.subWindowList();
        foreach(QMdiSubWindow *mdiWindow, mdiWindows) {
            if (DatabaseView *dbView =
                qobject_cast<DatabaseView *>(mdiWindow->widget())){
                if (dbView->hasTable(db->database(), folderName)) {
                    delete dbView;
                    mdiWindow->close();
                }
            }
        }

        // Remove all nodes for the given database.
        // Start from bottom to top to prevent shifting the indexes.
        for(int i = m_foldersList->topLevelItemCount() - 1; i >= 0 ; --i) {
            QTreeWidgetItem *item = m_foldersList->topLevelItem(i);
            if (item->text(Name) == folderName)
                m_foldersList->takeTopLevelItem(i);
        }

        // Finally remove table from the database.
        db->removeTable(folderName);
    }
}

void MainWindow::onParserStateChanged(int /*state*/)
{
    core::AtomParser *parser = core::Application::theApp()->streamParser();
    statusBar()->showMessage(parser->statusMessage(), 2000);
}

void MainWindow::onFolderDblClicked(const QModelIndex &index)
{
    core::Database *db = core::Application::theApp()->database();
    QTreeWidgetItem *item = m_foldersList->topLevelItem(index.row());
    QString tableName = item->text(Name);
    QList<QMdiSubWindow *> mdiWindows = m_mdiArea.subWindowList();
    foreach(QMdiSubWindow *mdiWindow, mdiWindows) {
        if (DatabaseView *dbView =
            qobject_cast<DatabaseView *>(mdiWindow->widget())){
            if (dbView->hasTable(db->database(), tableName)) {
                setActiveSubWindow(mdiWindow);
                return;
            }
        }
    }

    // If we reach here, the mdi child window wasn't found,
    // and we need to create new one.
    createFolderView(tableName);
}

} // namespace gui
