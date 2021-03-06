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

#ifndef __GENERALOPTIONSPAGE_H__
#define __GENERALOPTIONSPAGE_H__

#include "abstractOptionsPage.h"

class QCheckBox;
class QLineEdit;

namespace gui
{

/// This class implements an options dialog General page class.
class GeneralOptionsPage : public AbstractOptionsPage
{
    Q_OBJECT
public:
    /// Default constructor
    GeneralOptionsPage(QWidget *parent = 0, Qt::WFlags flags = 0);

    /// Destructor
    ~GeneralOptionsPage();

    /// Returns the name of the page.
    /*!
	    This name will appear in the navigation tree view.
    */
    virtual QString name() const;

    /// Save defaults associated with this page.
    virtual void saveDefaults();

private:
    /// Quit on close settings.
    QCheckBox *m_chkQuitOnClose;
};

} // namespace gui

#endif // __GENERALOPTIONSPAGE_H__
