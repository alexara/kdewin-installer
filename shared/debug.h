/****************************************************************************
**
** Copyright (C) 2008 Ralf Habacker. All rights reserved.
**
** This file is part of the KDE installer for windows
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License version 2 as published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public License
** along with this library; see the file COPYING.LIB.  If not, write to
** the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
** Boston, MA 02110-1301, USA.
**
****************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include <QtDebug>

#ifdef INLINE_DEBUG
inline QDebug kDebug() { QDebug a(QtDebugMsg); a << __FILE__ << __LINE__; return a; }
inline QDebug kInfo() { QDebug a(QtDebugMsg); a << __FILE__ << __LINE__; return a; }
inline QDebug kWarning() { QDebug a(QtWarningMsg); a << __FILE__ << __LINE__; return a; }
inline QDebug kCritial() { QDebug a(QtCriticalMsg); a << __FILE__ << __LINE__; return a; }
inline QDebug kFatal() { QDebug a(QtFatalMsg); a << __FILE__ << __LINE__; return a; }
#else
QDebug kDebug();
QDebug kInfo();
QDebug kWarning();
QDebug kCritial();
QDebug kFatal();
#endif

#endif
