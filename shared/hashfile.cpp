/****************************************************************************
**
** Copyright (C) 2006-2008 Ralf Habacker. All rights reserved.
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

#include "config.h"
#include "hash.h"
#include "hashfile.h"
#include "misc.h"

#include <QByteArray>
#include <QList>
#include <QFile>
#include <QFileInfo>

HashFile::HashFile(HashFile::Type type, const QString &fileName, const QString &basePath) 
    : Hash(type),
      m_fileName(fileName),
      m_fullName(basePath.isEmpty() ? fileName : basePath + '/' + fileName)
{
   
}

HashFile::~HashFile()
{
}

bool HashFile::computeHash()
{
    if(m_fullName.contains("\\ ")) 
    {
        m_fullName.replace("\\ ", " ");
    }
    
    QFile f(m_fullName);
    if (!f.open(QIODevice::ReadOnly)) 
    {
        qCritical() << "could not open file" << m_fullName << " error" << f.error();
        return false;
    }
    m_hash = hash(f);
    if (m_hash.isEmpty())
    {
        qCritical() << "could not compute hash value from " << m_fullName;
        return false;
    }
    return true;
}

QByteArray HashFile::toHashFileContent()
{
    if (m_hash.isEmpty())
        if (!computeHash())
            return QByteArray();
    
    return m_hash.toHex() + QByteArray("  ") + m_fileName.toUtf8() + QByteArray("\n");
}

bool HashFile::save(const QString &fileName)
{
    if (m_hash.isEmpty())
        if (!computeHash())
            return false;
    
    QFile f(!fileName.isEmpty() ? fileName : m_fullName + fileNameExtension());
    if (!f.open(QIODevice::WriteOnly)) 
    {
        qCritical() << "could not create hash file" << f.fileName();
        return false;
    }
    QFileInfo fi(m_fullName);
    const QByteArray hashFileContent = toHashFileContent(); 
    if (hashFileContent.isEmpty())
    {
        qCritical() << "could not create hash file content";
        return false;
    }
    f.write(hashFileContent); 
    f.close();
    return true;
}

bool HashFile::isHashFileName(const QString &fileName)
{
    return fileName.endsWith(QLatin1String(".md5")) || fileName.endsWith(QLatin1String(".sha1"));
}

bool HashFile::readFromFile(const QString &fileName)
{
    QFile f(!fileName.isEmpty() ? fileName : m_fullName + fileNameExtension());
    if (!f.open(QIODevice::ReadOnly)) 
        return false;
    const QList<QByteArray> parts = f.readLine().trimmed().split(' ');
    if (parts.size() != 3)
    {
        qCritical() << "invalid hash file format" << f.fileName();
        return false;
    }
    if (!isHash(parts[0]))
        return false;

    m_hash = parts[0];
    return true;
}

QString HashFile::fileNameExtension() const
{
    if (m_type == HashFile::MD5)
        return ".md5";
    else if (m_type == HashFile::SHA1)
        return ".sha1";
    else 
        return ".none";
}

QByteArray HashFile::getHash() const
{   
    return m_hash;
}
