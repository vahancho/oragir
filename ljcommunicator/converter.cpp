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

#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include "converter.h"

namespace xmlrpc
{

// xmlrpc scalar types. Refer to the spec (http://xmlrpc.scripting.com/spec.html)
const char tagInt[] = "int";
const char tagInt2[] = "i4";
const char tagBool[] = "boolean";              // 0 (false) or 1 (true)
const char tagString[] = "string";
const char tagDouble[] = "double";             // Double-precision signed floating point number
const char tagDateTime[] = "dateTime.iso8601"; // date/time 19980717T14:08:55
const char tagBase64[] = "base64";             // base64-encoded binary

// Other tags
const char tagValue[] = "value";
const char tagArray[] = "array";
const char tagStruct[] = "struct";
const char tagData[] = "data";
const char tagMember[] = "member";
const char tagName[] = "name";



}
