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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QMdiArea>
#include <QSystemTrayIcon>
#include <QtSql>
#include "blogEventView.h"

class QSignalMapper;
class QTreeWidget;
class QTreeWidgetItem;
class QTableView;
class QLabel;

namespace gui
{

class BlogTableModel;

/// Implements the Application's Main Window class.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// The constructor.
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);

    /// Destructor.
    ~MainWindow();

    /// Returns visibility state of the status bar.
    bool statusBarVisible() const;

    /// Create new blog model and set it to the blog view.
    /*!
        If model already exists, this function deletes it
        and creates a new one that will be set to the
        blog view.
    */
    void setupBlogView();

public slots:
    void onRecordInserted(const QString &table);

    /// This slot is called when Status bar action is triggered.
    void setStatusBarVisible(bool visible = true);

    /// Set up the table mdi child view for the given database table.
    void createFolderView(const QString &table);

protected:
    /// Handles the window's close event.
    /*!
        Saves MainWindow geometry and settings.
    */
    void closeEvent(QCloseEvent *event);

private slots:
    /// This slot is called when Tool bars sub menu is about to show.
    void updateToolBarsMenu();

    /// This slot is called when Views sub menu is about to show.
    void updateViewsMenu();

    /// This slot called when View menu is about to show.
    void setStatusBarActionState();

    /// This slot is called when Window menu is about to show.
    void updateWindowMenu();

    /// This slot called when a MDI sub window activated.
    /*!
        @param subWindow The MDI child window that has been activated.
        \sa setActiveSubWindow(), activeSubWindow()
    */
    void onSubWindowActivated(QMdiSubWindow *subWindow);

    /// This slot activates MDI sub window.
    /*!
        @param subWindow The MDI child window that will be activated.
        \sa activeSubWindow(), onSubWindowActivated()
    */
    void setActiveSubWindow(QWidget *subWindow);

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void trayIconMessageClicked();

    /// This slot called when About menu action triggered.
    void onAbout();

    void onHelp();

    void onStreamStart();

    /// Called when stream parsing stopped by any reason.
    void onStreamStopped();

    void onNewFolder();

    void onItemProcessed();

    void onDataReadProgress(int done, int total);

    void onFilters();

    void onFiltersExport();

    void onFiltersImport();

    /// Opens Options dialog.
    void onOptions();

    void onFolderContextMenu(const QPoint &pos);

    void onFolderDelete();

    void onParserStateChanged(int state);

    void onFolderDblClicked(const QModelIndex &);

     /// Updates status bar labels with items statistics.
    void updateStatusLabels(const QString &table);

    void onVersionChecked();

    /// Called when opening a table view action triggered.
    void onTableViewOpen();

    void onBlogAccountSetup();

    /// Called when an item in blog view clicked.
    void onEventClicked(const QModelIndex &index);

    void onEventOpen();

    void onBlogViewContextMenu(const QPoint &);

    /// Handle the user pics downloading request finished signal.
    void onNetManagerFinished(QNetworkReply *);

    /// Called when the new blog post action tigerred.
    void onNewPost();

    void onCommitChanges();

    void onSynchronize();

private:

    /// Defines the column numbers in the folders view.
    enum FolderViewColumn
    {
        Name = 0
    };

    /// Returns the full path to the user blog's database file.
    QString blogDataFile(const QString &user) const;

    /// Creates and sets up the folders tree view.
    void createFolderTree();

    /// Returns the active MDI sub window.
    /*!
        Returns the active MDI sub window if any, otherwise returns 0.
        \sa setActiveSubWindow(), onSubWindowActivated()
    */
    QMdiSubWindow *activeSubWindow() const;

    /// Create new mdi sub window with the given view and title.
    QMdiSubWindow *createSubWindow(QWidget *view,
                                   const QString &title,
                                   bool max = true);

    /// Construct and add menus.
    void createMenus();

    /// Create dockable windows.
    void createDockWindows();

    /// Creates the system tray icon.
    void createTrayIcon();

    void openTableView(const QString &tableName);

    /// Creates the blog view.
    void createBlogView();

    void updateBlogModel();

    /// Download the user's pictures by the given list of urls.
    void downloadUserPics(const QStringList &urls);

    /// Creates the initial blog event view with default settings.
    BlogEventView *createBlogEventView();

    /// The system tray icon.
    QSystemTrayIcon *m_trayIcon;

    /// Store the "Status bar" action.
    QAction *m_statusBarAction;

    /// Store the "Close" action.
    QAction *m_closeAct;

    /// Store the "Close all" action.
    QAction *m_closeAllAct;

    /// Store the "Tile" action.
    QAction *m_tileAct;

    /// Store the "Cascade" action.
    QAction *m_cascadeAct;

    /// Store the "Next" action.
    QAction *m_nextAct;

    /// Store the "Previous" action.
    QAction *m_previousAct;

    /// Store the separator action.
    QAction *m_separatorAct;

    QAction *m_startAction;

    QAction *m_stopAction;

    QAction *m_folderDeleteAction;

    BlogEventView::HtmlActions m_htmlActions;

    /// Store the Window menu.
    QMenu *m_windowMenu;

    /// Store the 'Toolbars' menu.
    QMenu *m_toolBarsMenu;

    /// Store the 'Views' menu.
    QMenu *m_viewsMenu;

    /// The signal mapper for MDI child windows.
    QSignalMapper *m_windowMapper;

    /// Workspace for MDI application.
    QMdiArea m_mdiArea;

    /// Stores the list of opened databases.
    QTreeWidget *m_folderTree;

    QTreeWidgetItem *m_filterFolder;

    /// Stores the blog view.
    QTableView *m_blogView;

    BlogTableModel *m_blogModel;

    /// The number of processed streaming items.
    int m_processedItemCount;

    /// The number of recoreded (filtered) items.
    int m_recordedItemCount;

    /// The streaming activity animation icon.
    QMovie *m_progress;

    /// Stores the status bar state.
    bool m_statusBarVisible;

    QLabel *m_processedItems;

    QLabel *m_processedData;

    QLabel *m_unreadItems;

    QLabel *m_totalItems;
};

} // namespace gui

#endif // __MAINWINDOW_H__
