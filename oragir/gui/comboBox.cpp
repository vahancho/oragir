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

#include <QListWidget>
#include <QLineEdit>
#include <QMouseEvent>
#include <QDebug>
#include "comboBox.h"

namespace gui
{

CheckModel::CheckModel(QObject *parent)
    :
        QStandardItemModel(parent)
{}

Qt::ItemFlags CheckModel::flags(const QModelIndex & index)
{
    return QStandardItemModel::flags(index) | Qt::ItemIsUserCheckable;
}

///////////////////////////////////////////////////////////////////////////////
ComboBox::ComboBox(QWidget *parent)
    :
        QComboBox(parent)
{
    connectTextChanged();
    view()->viewport()->installEventFilter(this);
    m_model = new CheckModel;
    connect(m_model, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
            this, SLOT(onItemInderted(const QModelIndex &, int, int)));
    setModel(m_model);
}

ComboBox::~ComboBox()
{
    delete m_model;
}

bool ComboBox::eventFilter(QObject *obj, QEvent *event)
{
     if (event->type() == QEvent::MouseButtonRelease) {
         int index = view()->currentIndex().row();

         disconnectTextChanged();

         if (itemData(index, Qt::CheckStateRole) == Qt::Checked)
            setItemData(index, Qt::Unchecked, Qt::CheckStateRole);
         else
            setItemData(index, Qt::Checked, Qt::CheckStateRole);

         updateText();

         connectTextChanged();

         return true;
     } else {
         // Propagate to the parent class.
         return QObject::eventFilter(obj, event);
     }
}

void ComboBox::onEditTextChanged(const QString &text)
{
    // Disconnect this slot to prevent recursive calls.
    disconnectTextChanged();

    // Uncheck all check boxes.
    for (int i = 0; i < count(); ++i) {
        setItemData(i, Qt::Unchecked, Qt::CheckStateRole);
    }

    QStringList itemList = text.split(',', QString::SkipEmptyParts);
    foreach (const QString &s, itemList) {
        int i = findText(s.trimmed());
        if (i != -1) {
            setItemData(i, Qt::Checked, Qt::CheckStateRole);
        }
    }

    setEditText(text);
    connectTextChanged();
}

void ComboBox::onItemInderted(const QModelIndex &parent, int start, int end)
{
    setItemData(start, Qt::Unchecked, Qt::CheckStateRole);
}

void ComboBox::updateText()
{
    QStringList itemList;
    for (int i = 0; i < count(); ++i) {
        if (itemData(i, Qt::CheckStateRole) == Qt::Checked)
            itemList.push_back(itemText(i));
    }
    setEditText(itemList.join(", "));
}

void ComboBox::disconnectTextChanged()
{
    disconnect(this, SIGNAL(editTextChanged(const QString &)),
               this, SLOT(onEditTextChanged(const QString &)));
}

void ComboBox::connectTextChanged()
{
    connect(this, SIGNAL(editTextChanged(const QString &)),
            this, SLOT(onEditTextChanged(const QString &)));
}

} // namespace gui
