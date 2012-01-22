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
#include "dlgUser.h"
#include "generalOptionsPage.h"
#include "connectOptionsPage.h"
#include "advancedOptionsPage.h"
#include "blogTableModel.h"
#include "../core/application.h"
#include "../core/defaultManager.h"
#include "../core/versionManager.h"
#include "../core/credentials.h"
#include "../parser/atomParser.h"
#include "../database/streamdatabase.h"
#include "../database/blogdatabase.h"
#include "../strings/strings.h"
#include "../strings/guiStrings.h"
#include "../../ljcommunicator/communicator.h"

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
    connect(parser, SIGNAL(stopped()),
            this, SLOT(onStreamStopped()));

    // Handle application upsates.
    connect(core::Application::theApp()->versionManager(),
            SIGNAL(checked()), this,
            SLOT(onVersionChecked()));

    createFolderTree();

    QLabel *statusLabel = new QLabel(this);
    statusLabel->setMinimumWidth(24);
    m_progress = new QMovie(":/icons/progress");
    statusLabel->setMovie(m_progress);
    statusBar()->addPermanentWidget(statusLabel);

    m_processedItems = new QLabel(this);
    m_processedItems->setText("  Pr 0  ");
    m_processedItems->setToolTip(str::ProcessedRecords);
    statusBar()->addPermanentWidget(m_processedItems);

    m_processedData = new QLabel(this);
    m_processedData->setText(" Size 0 kB  ");
    m_processedData->setToolTip(str::Downloaded);
    statusBar()->addPermanentWidget(m_processedData);

    m_unreadItems = new QLabel(this);
    m_unreadItems->setText("  Unread: 0  ");
    m_unreadItems->setToolTip(str::Unread);
    statusBar()->addPermanentWidget(m_unreadItems);

    m_totalItems = new QLabel(this);
    m_totalItems->setText("  Total: 0  ");
    m_totalItems->setToolTip(str::Total);
    statusBar()->addPermanentWidget(m_totalItems);
}

// Destructor
MainWindow::~MainWindow()
{
    delete m_blogModel;
}

void MainWindow::createBlogView()
{
    m_blogView = new QTableView;
    m_blogView->setShowGrid(false);
    m_blogView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_blogView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_blogView->verticalHeader()->setVisible(false);
    m_blogView->setSortingEnabled(true);
    // The uniformed rows height.
    QFontMetrics fm = fontMetrics();
    m_blogView->verticalHeader()->setDefaultSectionSize(fm.height() + 6);

    connect(m_blogView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(onEventClicked(const QModelIndex &)));

    core::BlogDatabase *db = core::Application::theApp()->blogDatabase();
    QSqlDatabase d = db->database();
    if (d.isValid()) {
        m_blogModel = new BlogTableModel(0, db->database());
        m_blogModel->setTable(str::MyBlogTableName);
        m_blogModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        m_blogModel->select();

        m_blogView->setModel(m_blogModel);

        for (int i = 0; i < m_blogModel->columnCount(); ++i) {
            if (i != BlogTableModel::Subject &&
                i != BlogTableModel::Time) {
                m_blogView->hideColumn(i);
            }
        }
    }

    QDockWidget *dock = new QDockWidget("My Blog", this);
    dock->setObjectName("My Blog");
    dock->setWidget(m_blogView);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::createFolderTree()
{
    m_folderTree = new QTreeWidget(this);
    m_folderTree->setColumnCount(1);
    QStringList headerLabels;
    headerLabels << QString();
    m_folderTree->setHeaderLabels(headerLabels);
    m_folderTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_folderTree, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(onFolderContextMenu(const QPoint &)));
    connect(m_folderTree, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(onFolderDblClicked(const QModelIndex &)));

    // Create default folders.
    m_filterFolder = new QTreeWidgetItem;
    m_filterFolder->setIcon(Name, QIcon(":/icons/folder"));
    m_filterFolder->setText(Name, "Filtered");
    m_filterFolder->setToolTip(Name, "Filtered");
    m_folderTree->addTopLevelItem(m_filterFolder);

    QDockWidget *dock = new QDockWidget(str::FoldersTitle, this);
    dock->setObjectName(str::FoldersTitle);
    dock->setWidget(m_folderTree);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::createFolderView(const QString &table)
{
    core::StreamDatabase *dbObj = core::Application::theApp()->streamDatabase();
    QSqlDatabase db = dbObj->database();
    if (!db.isValid())
        return;

    // Duplicate nodes disallowed for filter folders, so search
    // among existing nodes and if a node with the given name exists
    // do not add another one.
    bool folderExist = false;
    for (int i = 0; i < m_filterFolder->childCount(); ++i) {
        QTreeWidgetItem *node = m_filterFolder->child(i);
        if (node->text(Name) == table) {
            folderExist = true;
            break;
        }
    }

    if (!folderExist) {
        // Add tree node for the given database.
        QTreeWidgetItem *node = new QTreeWidgetItem(m_filterFolder);
        node->setIcon(Name, QIcon(":/icons/folder"));
        node->setText(Name, table);
        node->setToolTip(Name, table);
        node->setData(Name, Qt::UserRole, table);
        m_folderTree->addTopLevelItem(node);
        m_folderTree->setCurrentItem(node);
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
    connect(dbView, SIGNAL(changed(const QString &)), this,
            SLOT(updateStatusLabels(const QString &)));
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

void MainWindow::onHelp()
{
    QDesktopServices::openUrl(QUrl(str::HelpUrl, QUrl::TolerantMode));
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

    m_folderDeleteAction = fileMenu->addAction(QIcon(":/icons/folder_delete"),
                                               str::ActionDelete,
                                               this,
                                               SLOT(onFolderDelete()));
    fileToolBar->addAction(m_folderDeleteAction);

    fileMenu->addSeparator();
    QAction *quitAction = fileMenu->addAction(str::ActionExit);
    quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    quitAction->setIcon(QIcon(":icons/exit"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    ///////////////////////////////////////////////////////////////////////////
    // Edit menu
    //
    QMenu *editMenu = new QMenu("&Edit", this);
    QToolBar *editToolBar = new QToolBar("&Edit", this);
    editToolBar->setObjectName("&Edit");
    editToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    QAction *action = editMenu->addAction("Undo");
    action->setIcon(QIcon(":icons/undo"));
    m_htmlActions[BlogEventView::Undo] = action;
    editToolBar->addAction(action);
    action = editMenu->addAction("Redo");
    action->setIcon(QIcon(":icons/redo"));
    m_htmlActions[BlogEventView::Redo] = action;
    editToolBar->addAction(action);
    editMenu->addSeparator();
    action = editMenu->addAction("Cu&t");
    action->setIcon(QIcon(":icons/cut"));
    m_htmlActions[BlogEventView::Cut] = action;
    editToolBar->addAction(action);
    action = editMenu->addAction("&Copy");
    action->setIcon(QIcon(":icons/copy"));
    m_htmlActions[BlogEventView::Copy] = action;
    editToolBar->addAction(action);
    action = editMenu->addAction("&Paste");
    action->setIcon(QIcon(":icons/paste"));
    m_htmlActions[BlogEventView::Paste] = action;
    editToolBar->addAction(action);
    editMenu->addSeparator();
    action = editMenu->addAction("Select All");
    action->setIcon(QIcon(":icons/select_all"));
    m_htmlActions[BlogEventView::SelectAll] = action;
    editToolBar->addAction(action);

    ///////////////////////////////////////////////////////////////////////////
    // Format menu
    //
    QMenu *formatMenu = new QMenu("&Format", this);
    QToolBar *formatToolBar = new QToolBar("&Format", this);
    formatToolBar->setObjectName("&Format");
    formatToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    QMenu *styleMenu = formatMenu->addMenu("Style");
    QToolBar *styleToolBar = new QToolBar("Style", this);
    styleToolBar->setObjectName("Style");
    styleToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    action = styleMenu->addAction("Paragraph");
    action->setIcon(QIcon(":icons/paragraph"));
    m_htmlActions[BlogEventView::Paragraph] = action;
    styleToolBar->addAction(action);
    action = styleMenu->addAction("Heading 1");
    action->setIcon(QIcon(":icons/h1"));
    m_htmlActions[BlogEventView::Heading1] = action;
    styleToolBar->addAction(action);
    action = styleMenu->addAction("Heading 2");
    action->setIcon(QIcon(":icons/h2"));
    m_htmlActions[BlogEventView::Heading2] = action;
    styleToolBar->addAction(action);
    action = styleMenu->addAction("Heading 3");
    action->setIcon(QIcon(":icons/h3"));
    m_htmlActions[BlogEventView::Heading3] = action;
    styleToolBar->addAction(action);
    action = styleMenu->addAction("Heading 4");
    action->setIcon(QIcon(":icons/h4"));
    m_htmlActions[BlogEventView::Heading4] = action;
    styleToolBar->addAction(action);
    action = styleMenu->addAction("Heading 5");
    action->setIcon(QIcon(":icons/h5"));
    m_htmlActions[BlogEventView::Heading5] = action;
    styleToolBar->addAction(action);
    action = styleMenu->addAction("Heading 6");
    action->setIcon(QIcon(":icons/h6"));
    m_htmlActions[BlogEventView::Heading6] = action;
    styleToolBar->addAction(action);
    action = styleMenu->addAction("Address");
    //action->setIcon(QIcon(":icons/address"));
    m_htmlActions[BlogEventView::Address] = action;
    styleToolBar->addAction(action);

    QMenu *alignMenu = formatMenu->addMenu("Align");
    QToolBar *alignToolBar = new QToolBar("Align", this);
    alignToolBar->setObjectName("Align");
    alignToolBar->setIconSize(QSize(iconSizeX, iconSizeY));

    action = alignMenu->addAction("Align Left");
    action->setIcon(QIcon(":icons/align_left"));
    m_htmlActions[BlogEventView::AlignLeft] = action;
    alignToolBar->addAction(action);
    action = alignMenu->addAction("Align Center");
    action->setIcon(QIcon(":icons/align_center"));
    m_htmlActions[BlogEventView::AlignCenter] = action;
    alignToolBar->addAction(action);
    action = alignMenu->addAction("Align Right");
    action->setIcon(QIcon(":icons/align_right"));
    m_htmlActions[BlogEventView::AlignRight] = action;
    alignToolBar->addAction(action);
    action = alignMenu->addAction("Align Justify");
    action->setIcon(QIcon(":icons/align_just"));
    m_htmlActions[BlogEventView::AlignJustify] = action;
    alignToolBar->addAction(action);

    formatMenu->addSeparator();
    action = formatMenu->addAction("Bold");
    action->setIcon(QIcon(":icons/bold"));
    m_htmlActions[BlogEventView::Bold] = action;
    formatToolBar->addAction(action);
    action = formatMenu->addAction("Italic");
    action->setIcon(QIcon(":icons/italic"));
    m_htmlActions[BlogEventView::Italic] = action;
    formatToolBar->addAction(action);
    action = formatMenu->addAction("Underline");
    action->setIcon(QIcon(":icons/underline"));
    m_htmlActions[BlogEventView::Underline] = action;
    formatToolBar->addAction(action);
    action = formatMenu->addAction("Srikethrough");
    action->setIcon(QIcon(":icons/strike"));
    action->setCheckable(true);
    m_htmlActions[BlogEventView::Srikethrough] = action;
    formatToolBar->addAction(action);
    formatMenu->addSeparator();
    action = formatMenu->addAction("Increase Indent");
    action->setIcon(QIcon(":icons/indent"));
    m_htmlActions[BlogEventView::IncreaseIndent] = action;
    formatToolBar->addAction(action);
    action = formatMenu->addAction("Decrease Indent");
    action->setIcon(QIcon(":icons/outdent"));
    m_htmlActions[BlogEventView::DecreaseIndent] = action;
    formatToolBar->addAction(action);
    formatMenu->addSeparator();
    action = formatMenu->addAction("Numbered List");
    action->setIcon(QIcon(":icons/numbered"));
    action->setCheckable(true);
    m_htmlActions[BlogEventView::NumberedList] = action;
    formatToolBar->addAction(action);
    action = formatMenu->addAction("Bulleted List");
    action->setIcon(QIcon(":icons/bullet"));
    action->setCheckable(true);
    m_htmlActions[BlogEventView::BulletedList] = action;
    formatToolBar->addAction(action);
    formatMenu->addSeparator();
    action = formatMenu->addAction("Font Name...");
    action->setIcon(QIcon(":icons/font"));
    m_htmlActions[BlogEventView::FontName] = action;
    formatToolBar->addAction(action);
    action = formatMenu->addAction("Font Size...");
    action->setIcon(QIcon(":icons/font_size"));
    m_htmlActions[BlogEventView::FontSize] = action;
    formatToolBar->addAction(action);
    formatMenu->addSeparator();
    action = formatMenu->addAction("Text Color...");
    action->setIcon(QIcon(":icons/text_color"));
    m_htmlActions[BlogEventView::TextColor] = action;
    formatToolBar->addAction(action);
    action = formatMenu->addAction("Background Color...");
    action->setIcon(QIcon(":icons/bg_color"));
    m_htmlActions[BlogEventView::BgColor] = action;
    formatToolBar->addAction(action);

    //////////////////////////////////////////////////////////////////////////
    // View menu
    //
    QMenu *viewMenu = new QMenu(str::MenuView, this);
    connect(viewMenu, SIGNAL(aboutToShow()), this, SLOT(setStatusBarActionState()));

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
    connect(m_stopAction, SIGNAL(triggered()),
            core::Application::theApp()->streamParser(), SLOT(stop()));
    m_stopAction->setIcon(QIcon(":icons/stop"));
    m_stopAction->setEnabled(false);
    streamToolBar->addAction(m_stopAction);
    streamMenu->addSeparator();

    QAction *filtersAction = streamMenu->addAction(str::ActionFilters);
    connect(filtersAction, SIGNAL(triggered()), this, SLOT(onFilters()));
    filtersAction->setIcon(QIcon(":icons/filter"));
    streamToolBar->addAction(filtersAction);

    QAction *fltExportAction = streamMenu->addAction(str::ActionFilterExport);
    connect(fltExportAction, SIGNAL(triggered()), this, SLOT(onFiltersExport()));
    QAction *fltImportAction = streamMenu->addAction(str::ActionFilterImport);
    connect(fltImportAction, SIGNAL(triggered()), this, SLOT(onFiltersImport()));

    //////////////////////////////////////////////////////////////////////////
    // Blog menu
    //
    QMenu *blogMenu = new QMenu("&Blog", this);
    QAction *setupAction = blogMenu->addAction("&Setup Account...");
    connect(setupAction, SIGNAL(triggered()), this, SLOT(onBlogAccountSetup()));

    //////////////////////////////////////////////////////////////////////////
    // Tools menu
    //
    QMenu *toolsMenu = new QMenu(str::MenuTools, this);

    QAction *optionsAction = toolsMenu->addAction(str::ActionOptions);
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(onOptions()));
    optionsAction->setIcon(QIcon(":icons/options"));

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
    QAction *aboutHelp = helpMenu->addAction(str::ActionHelp);
    connect(aboutHelp, SIGNAL(triggered()), this, SLOT(onHelp()));
    QAction *aboutAction = helpMenu->addAction(str::ActionAbout);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(onAbout()));

    //////////////////////////////////////////////////////////////////////////

    // Add menus to menu bar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(formatMenu);
    menuBar()->addMenu(streamMenu);
    menuBar()->addMenu(blogMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(m_windowMenu);
    menuBar()->addSeparator();
    menuBar()->addMenu(helpMenu);

    // Add tool bars
    addToolBar(fileToolBar);
    addToolBar(editToolBar);
    addToolBar(formatToolBar);
    addToolBar(styleToolBar);
    addToolBar(alignToolBar);
    addToolBar(streamToolBar);
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

    QWidget *widget = subWindow->widget();

    if (DatabaseView *dbView = qobject_cast<DatabaseView *>(widget)) {
        updateStatusLabels(dbView->table());
    } else if (BlogEventView *eventView = qobject_cast<BlogEventView *>(widget)) {
        eventView->setupActions(m_htmlActions);
    }
}

void MainWindow::updateStatusLabels(const QString &table)
{
    core::StreamDatabase *db = core::Application::theApp()->streamDatabase();
    int unreadCount = db->unreadCount(table);

    // Update status labels only for the active table view.
    if (QMdiSubWindow *mdiWindow = m_mdiArea.activeSubWindow()) {
        if(DatabaseView *dbView =
           qobject_cast<DatabaseView *>(mdiWindow->widget())) {
            if (dbView->table() == table) {
                m_unreadItems->setText(QString("  Unread: %1  ")
                                       .arg(unreadCount));
                m_totalItems->setText(QString("  Total: %1  ")
                                      .arg(db->totalCount(table)));
            }
        }
    }

    // Update the corresponding folder (table) name with the number
    // of unread items.
    for(int i = 0; i < m_filterFolder->childCount(); ++i) {
        QTreeWidgetItem *item = m_filterFolder->child(i);
        QString name = item->data(Name, Qt::UserRole).toString();
        if (name == table) {
            QFont f;
            if (unreadCount > 0) {
                name += QString(" (%1)").arg(unreadCount);
                f.setBold(true);
            } else {
                f.setBold(false);
            }
            item->setText(Name, name);
            item->setData(Name, Qt::FontRole, f);
            break;
        }
    }
}

void MainWindow::setActiveSubWindow(QWidget *subWindow)
{
    if (!subWindow)
        return;

    m_mdiArea.setActiveSubWindow(qobject_cast<QMdiSubWindow *>(subWindow));
}

void MainWindow::onRecordInserted(const QString &table)
{
    // Find database view that has to be updated.
    QList<QMdiSubWindow *> mdiWindows = m_mdiArea.subWindowList();
    foreach(QMdiSubWindow *mdiWindow, mdiWindows) {
        if (DatabaseView *dbView =
            qobject_cast<DatabaseView *>(mdiWindow->widget())){
            if (dbView->table() == table) {
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

    // Disable start action until streaming is stopped.
    m_startAction->setEnabled(false);
    m_stopAction->setEnabled(true);
    m_progress->start();
    // Reset the counter.
    m_processedItemCount = 0;
}

void MainWindow::onStreamStopped()
{
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
        core::StreamDatabase *db = core::Application::theApp()->streamDatabase();
        if (db->addStreamTable(table)) {
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
    core::StreamDatabase *db = core::Application::theApp()->streamDatabase();
    FiltersDialog dlg;
    const core::StreamDatabase::Filters &filters = db->filters();
    dlg.setFilters(filters);
    if (dlg.exec() == QDialog::Accepted) {
        // Read the filters from dialog and update database.
        const core::StreamDatabase::Filters &tmpFilters = dlg.filters();
        core::StreamDatabase::Filters::const_iterator it = tmpFilters.begin();
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
        QFileDialog::getSaveFileName(this, str::ExportFilterTitle,
                                     ".",
                                     str::FilterDialogFilter);
    if(!fileName.isEmpty()) {
        core::StreamDatabase *db = core::Application::theApp()->streamDatabase();
        db->saveFilters(fileName);
    }
}

void MainWindow::onFiltersImport()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, str::ImportFilterTitle,
                                     ".",
                                     str::FilterDialogFilter);
    if(!fileName.isEmpty()) {
        core::StreamDatabase *db = core::Application::theApp()->streamDatabase();
        // Backup the filters in case of the loading errors.
        core::StreamDatabase::Filters filters = db->filters();
        if (!db->openFilters(fileName)) {
            QMessageBox::critical(this, str::DatabaseError,
                                  db->errorMessage());

            // Restore filters back.
            foreach(const core::Filter<core::Post> &filter, filters) {
                db->addFilter(filter);
            }
        }
    }
}

void MainWindow::onOptions()
{
    // Create and show dialog.
    OptionsDialog dlg(this);
    dlg.addPage(new gui::GeneralOptionsPage);
    dlg.addPage(new gui::ConnectOptionsPage);
    dlg.addPage(new gui::AdvancedOptionsPage);
    // If dialog accepted, i.e. Ok button pressed, save all defaults.
    if(dlg.exec() == QDialog::Accepted)
        dlg.saveDefaults();
}

void MainWindow::onFolderContextMenu(const QPoint &pos)
{
    if(QTreeWidgetItem *node = m_folderTree->itemAt(pos)) {
        if (node->parent() == m_filterFolder) {
            QMenu menu;
            QAction *act = menu.addAction("&Open", this, SLOT(onTableViewOpen()));
            menu.setDefaultAction(act);
            menu.addAction(m_folderDeleteAction);

            menu.exec(m_folderTree->mapToGlobal(QPoint(pos.x(), pos.y() + 20)));
        }
    }
}

void MainWindow::onTableViewOpen()
{
    QString folderName =
        m_folderTree->currentItem()->data(Name, Qt::UserRole).toString();
    openTableView(folderName);
}

void MainWindow::onFolderDelete()
{
    QString folderName = m_folderTree->currentItem()->data(Name, Qt::UserRole).toString();

    // Find database view(s) that has to be closed.
    QList<QMdiSubWindow *> mdiWindows = m_mdiArea.subWindowList();
    foreach(QMdiSubWindow *mdiWindow, mdiWindows) {
        if (DatabaseView *dbView =
            qobject_cast<DatabaseView *>(mdiWindow->widget())){
            if (dbView->table() == folderName) {
                delete dbView;
                mdiWindow->close();
            }
        }
    }

    // Remove all nodes for the given database.
    // Start from bottom to top to prevent shifting the indexes.
    for(int i = m_filterFolder->childCount() - 1; i >= 0 ; --i) {
        QTreeWidgetItem *item = m_filterFolder->child(i);
        if (item->data(Name, Qt::UserRole) == folderName)
            m_filterFolder->takeChild(i);
    }

    // Finally remove table from the database.
    core::StreamDatabase *db = core::Application::theApp()->streamDatabase();
    db->removeTable(folderName);
}

void MainWindow::onParserStateChanged(int /*state*/)
{
    core::AtomParser *parser = core::Application::theApp()->streamParser();
    statusBar()->showMessage(parser->statusMessage(), 2000);
}

void MainWindow::onFolderDblClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QTreeWidgetItem *item = m_filterFolder->child(index.row());
        QString tableName = item->data(Name, Qt::UserRole).toString();
        openTableView(tableName);
    }
}

void MainWindow::openTableView(const QString &tableName)
{
    QList<QMdiSubWindow *> mdiWindows = m_mdiArea.subWindowList();
    foreach(QMdiSubWindow *mdiWindow, mdiWindows) {
        if (DatabaseView *dbView =
            qobject_cast<DatabaseView *>(mdiWindow->widget())){
            if (dbView->table() == tableName) {
                setActiveSubWindow(mdiWindow);
                return;
            }
        }
    }

    // If we reach here, the mdi child window wasn't found,
    // and we need to create new one.
    createFolderView(tableName);
}

void MainWindow::onVersionChecked()
{
    core::VersionManager *vm = core::Application::theApp()->versionManager();
    if (vm->updatesAvailable()) {
        QString title = QString("New Version Available");
        QString text = QString("<b>A new version %1 of %2 is available.</b>")
                              .arg(vm->updatedVersion())
                              .arg(str::AppName);

        QMessageBox msgBox;
        msgBox.setWindowTitle(title);
        msgBox.setText(text);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setInformativeText("Would you like to download and install it?");
        QAbstractButton *btnYes = msgBox.addButton("Update Now", QMessageBox::YesRole);
        msgBox.addButton("Cancel", QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton() == btnYes) {
            vm->download();
        }
    }
}

void MainWindow::onBlogAccountSetup()
{
    core::Credentials *cr = core::Application::theApp()->credentials();

    UserAccount dlg(this);
    dlg.setUser(cr->user());
    dlg.setPassword(cr->password());

    if (dlg.exec() == QDialog::Accepted) {
        m_progress->start();
        QProgressBar progressBar;
        progressBar.setMaximumWidth(150);
        progressBar.setMaximumHeight(16);
        progressBar.setMinimum(0);
        statusBar()->insertPermanentWidget(0, &progressBar);

        QString user = dlg.user();
        QString password = dlg.password();

        lj::Communicator com;
        com.setUser(user, password);

        // Verify user name and password
        lj::UserInfo userInfo = com.login();
        if (userInfo.isValid()) {
            // Password is correct, so store it.
            cr->setUser(user);
            cr->setPassword(password);

            core::BlogDatabase *db = core::Application::theApp()->blogDatabase();
            if (!db->database().isOpen()) {
                QString dbPath =
                    core::Application::theApp()->settingsDirectory() +
                    "users" + '/' + user + '/';
                dbPath = QDir::toNativeSeparators(dbPath);
                QDir dir(dbPath);
                if (!dir.exists())
                    dir.mkpath(dbPath);
                dbPath += user + ".data";
                db->create(dbPath);
            } else {
                QStringList tables = db->tables();
                if (tables.contains(str::MyBlogTableName)) {
                    // Clear the old table.
                    m_blogModel->removeRows(0, m_blogModel->rowCount());
                    m_blogModel->submitAll();
                }
            }

            // Create new table for the blog events if it does not exist.
            // No problem if the table already exists.
            QString query = QString(str::SqlCreateMyBlogTable)
                                    .arg(str::MyBlogTableName);
            db->addTable(query);

            // Create the user info table and add user data.
            db->addTable(str::SqlCreateMyBlogUserTable);
            db->setUserData(userInfo, cr->encode());

            // Get the total number of events in the blog.
            int total = 0;
            QVariantList postPerDay = com.getDayCount();
            foreach (const QVariant &v, postPerDay) {
                QMap<QString, QVariant> data = v.toMap();
                total += data["count"].toInt();
            }
            progressBar.setMaximum(total);

            // Resulting events.
            lj::Events events;
            int canFetch = 0;
            foreach (const QVariant &v, postPerDay) {
                // No need to fetch the same events twice.
                if (events.count() >= total)
                    break;

                QMap<QString, QVariant> data = v.toMap();
                int count = data["count"].toInt();
                canFetch += count;
                if (canFetch >= 50) {
                    QString dateStr = data["date"].toString();

                    lj::Events e = com.getDayEvents(dateStr);
                    if (e.isValid()) {
                        canFetch -= e.count();
                        events += e;
                        progressBar.setValue(events.count());
                    }

                    dateStr += " 00:00:00";
                    e = com.getEvents(canFetch, dateStr);
                    if (e.isValid()) {
                        canFetch -= e.count();
                        events += e;
                        progressBar.setValue(events.count());
                    }
                }
            }

            // Get the last n events.
            lj::Events e = com.getEvents(total - events.count());
            if (e.isValid()) {
                events += e;
            }

            // Finally add all events to the database.
            for (int i = 0; i < events.count(); ++i) {
                lj::Event event = events.event(i);
                db->addEvent(event);
            }

            progressBar.setValue(events.count());
        } else {
            QMessageBox::critical(this, "User Account Error",
                                  userInfo.error());
        }

        m_progress->stop();
    }
}

void MainWindow::onEventClicked(const QModelIndex &index)
{
    // Create and show database view.
    BlogEventView *view = new BlogEventView;
    QSqlRecord record = m_blogModel->record(index.row());
    QString subject = record.value(BlogTableModel::Subject).toString();
    view->setSubject(subject);
    view->setHtmlContent(record.value(BlogTableModel::Event).toString());
    view->setDateTime(QDateTime(record.value(BlogTableModel::Time).toDateTime()));
    view->setDateOutOrder(record.value(BlogTableModel::Backdated).toBool());

    // Get the user information from the user table.
    core::BlogDatabase *db = core::Application::theApp()->blogDatabase();
    QSqlTableModel userModel(0, db->database());
    userModel.setTable("user");
    userModel.select();
    QSqlRecord userRecord = userModel.record(0);

    QStringList journals = userRecord.value(3).toString().split(',');
    journals.prepend(core::Application::theApp()->credentials()->user());
    view->setPostTo(journals);

    QStringList userPics = userRecord.value(6).toString().split(',');
    userPics.prepend("default");
    view->setUserPics(userPics);

    QMdiSubWindow *editorView = new QMdiSubWindow;
    editorView->setWidget(view);
    editorView->setAttribute(Qt::WA_DeleteOnClose);
    editorView->resize(200, 200);
    editorView->setWindowTitle(subject);
    m_mdiArea.addSubWindow(editorView);
    editorView->showMaximized();
}

} // namespace gui
