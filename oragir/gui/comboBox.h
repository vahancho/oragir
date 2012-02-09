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

#ifndef __COMBOBOX_H__
#define __COMBOBOX_H__

#include <QComboBox>
#include <QStandardItemModel>

namespace gui
{

/// Implements the custom model for combo box view.
class CheckModel : public QStandardItemModel
{
public:
    CheckModel(QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &index);
};


/// Implements custom combo box that has checkable items.
/*!
    Changing check boxes states changes the combo box edit text
    with comma separated checked items texts.
    Changing edit text updates the check boxes states.
*/
class ComboBox : public QComboBox
{
    Q_OBJECT

public:
    ComboBox(QWidget *parent = 0);

    ~ComboBox();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void onEditTextChanged(const QString &);
    void onItemInderted(const QModelIndex &, int, int);

private:
    void updateText();

    /// Disconnects editTextChanged() signal from its receiver.
    void disconnectTextChanged();

    /// Connects editTextChanged() signal to its receiver.
    void connectTextChanged();

    CheckModel *m_model;
};

} // namespace gui

#endif // __COMBOBOX_H__
