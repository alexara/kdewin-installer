/****************************************************************************
**
** Copyright (C) 2011 Patrick von Reth <patrick.vonreth@gmail.com>
** Copyright (C) 2011 Ralf Habacker <ralf.habacker@freenet.de>
** All rights reserved.
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

#include "typehelper.h"
#include "misc.h"

#include <QMap>
#include <QRegExp>

CompilerTypes supportedCompilers(CompilerTypes::supportedCompiler);
CompilerTypes allCompilers(CompilerTypes::allCompiler);

//ReleaseType toReleaseType(const QString &_type)
//{
//    QString type(_type.toLower());
//    if (type == "unstable")
//        return Unstable;
//    else if (type== "stable")
//        return Stable;
//    else if (type == "nightly")
//        return Nightly;
//    else 
//        return RTUnspecified;
//}
//
//const QString toString(ReleaseType type)
//{
//    if (type == Stable)
//        return "stable";
//    else if (type == Unstable)
//	    return "unstable";
//    else if (type == Nightly)
//	    return "nightly";
//    else
//	    return "unknown";
//}

CompilerTypes::CompilerTypes(Scope scope) :  m_containsRegExp(0), m_endsRegExp(0)
{
    // see doc/format-specifications.txt for details
#ifdef BUILD_WITH_DEPRECATED
    m_types << MSVC9 << MinGW4;
    m_typeStrings << "vc90" << "mingw4";
    m_descriptions << "MSVC 2008 32bit (deprecated)" << "MinGW4 (deprecated)";
#endif
    m_types << MSVC10 << MSVC11 << MSVC12 << MSVC14 << MinGW4_W32;
    m_typeStrings << "vc100" << "vc110" << "vc120" << "vc140" << "x86-mingw4";
    m_descriptions << "MSVC 2010 32bit" << "MSVC 2011 32bit" << "MSVC 2012 32bit" << "MSVC 2014 32bit" << "MinGW4 32bit";
    if (scope == allCompiler || isX64Windows())
    {
    // @todo: enable msvc x64 builds if we ever set them up
#ifdef MSVC_X64
        m_types << MSVC10_X64;
        m_typeStrings << "x64-vc100";
        m_descriptions << "MSVC 2010 64bit";
#endif
        m_types << MinGW4_W64;
        m_typeStrings << "x64-mingw4";
        m_descriptions << "MinGW4 64bit";
    }
    m_containsRegExp = new QRegExp("("+values().join("|")+")");
    m_endsRegExp = new QRegExp("("+values().join("|")+")$");
 }

CompilerTypes::~CompilerTypes()
{
    delete m_containsRegExp;
    delete m_endsRegExp;
}

bool CompilerTypes::contains(Type type)
{
    return m_types.contains(type);
}

CompilerTypes::Type CompilerTypes::fromString(const QString &type)
{
    int i = m_typeStrings.indexOf(type.toLower());
    return i != -1 ? m_types[i] : CompilerTypes::Unspecified;
}

const QString CompilerTypes::toString(Type type)
{
    int i = m_types.indexOf(type);
    return i != -1 ? m_typeStrings[i] : QString();
}

const QStringList &CompilerTypes::values()
{
    return m_typeStrings;
}

const QString CompilerTypes::description(Type type)
{
    int i = m_types.indexOf(type);
    return i != -1 ? m_descriptions[i] : QString();
}

QRegExp &CompilerTypes::regex()
{
    return *m_containsRegExp;
}

QRegExp &CompilerTypes::endswith()
{
    return *m_endsRegExp;
}

FileTypes::Type FileTypes::fromString(const QString &type)
{
    static QMap<QString,FileTypes::Type> typeMap;
    if(typeMap.isEmpty()){
        typeMap.insert("bin",BIN);
        typeMap.insert("lib",LIB);
        typeMap.insert("doc",DOC);
        typeMap.insert("src",SRC);
        typeMap.insert("dbg",DBG);
        typeMap.insert("meta",META);
    }
    QString _type = type.toLower();
    return typeMap.contains(_type)?typeMap.value(_type):NONE;
}

const QString FileTypes::toString(Type type)
{
  switch(type) {
    case BIN:    return "bin";
    case LIB:    return "lib";
    case DOC:    return "doc";
    case SRC:    return "src";
    case DBG:    return "dbg";
    case META:   return "meta";
    case ALL:    return "all";
    default:     return "unknown";
    }
}

const QStringList FileTypes::values()
{
    // see doc/format-specifications.txt for details
    static QStringList list;
    if(list.isEmpty()){
        list<<"bin"<<"lib"<<"doc"<<"src"<<"dbg";
    }
    return list;
}

QRegExp &FileTypes::regex()
{
    static QRegExp compilersRx("("+FileTypes::values().join("|")+")");
    return compilersRx;
}

QRegExp &FileTypes::endswith()
{
    static QRegExp compilersRx("("+FileTypes::values().join("|")+")$");
    return compilersRx;
}