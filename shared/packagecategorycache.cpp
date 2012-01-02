/****************************************************************************
**
** Copyright (C) 2005-2007 Ralf Habacker. All rights reserved.
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

#include "packagecategorycache.h"
#include "packagelist.h"
#include "packageinfo.h"
#include "database.h"

PackageCategoryCache::PackageCategoryCache()
{
}

void PackageCategoryCache::setNote(const QString &category, const QString &notes)
{
    m_data[category].notes = notes;
}

void PackageCategoryCache::addPackage(Package *pkg)
{
    Q_FOREACH(const QString &category, pkg->categories())
    {
        if (!m_data[category].packages.contains(pkg->name()))
            m_data[category].packages.append(pkg->name());
    }
}

void PackageCategoryCache::addPackage(const QString &category, const QString &pkgName)
{
    if ( !m_data[category].packages.contains(pkgName) )
        m_data[category].packages.append(pkgName);
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}

QStringList PackageCategoryCache::categories()
{
    QStringList keys;
    QMap<QString, CategoryListType>::ConstIterator i = m_data.begin();
    for (;i != m_data.end(); ++i) 
    {
        if (!i.key().isEmpty())
            keys << i.key() + ":" + m_data[i.key()].notes;
    }
    qSort(keys.begin(), keys.end(), caseInsensitiveLessThan);
    return keys;
}

QList <Package *>PackageCategoryCache::packages(const QString &category, PackageList &list)
{
    QList <Package *> packages;
    if (!m_data.contains(category))
        return packages;
    Q_FOREACH(const QString &pkgName, m_data[category].packages) 
    {
        PackageInfo info;
        info.name = pkgName;
        Package *p = list.find(info);
        if (p)
            packages.append(p);
    }
    return packages;
}

QList <Package *>PackageCategoryCache::packages(const QString &category, Database &list)
{
    QList <Package *> packages;
    if (!m_data.contains(category))
        return packages;
    Q_FOREACH(const QString &pkgName, m_data[category].packages) 
    {
        Package *p = list.getPackage(pkgName);
        if (p)
            packages.append(p);
    }
    return packages;
}

void PackageCategoryCache::clear()
{
    m_data.clear();
}


QDebug &operator<<(QDebug &out, const PackageCategoryCache &c)
{
    out << "PackageCategoryCache ("
        << "size:" << c.m_data.size();
    QMap<QString, PackageCategoryCache::CategoryListType>::ConstIterator i = c.m_data.begin();
    for (;i != c.m_data.end(); ++i) 
        out << "( category:" << i.key() << "notes:" << c.m_data[i.key()].notes << "packages:" << c.m_data[i.key()].packages << ")";
    out << ")";
    return out;
}

PackageCategoryCache categoryCache;
