/****************************************************************************
**
** Copyright (C) 2006 Ralf Habacker. All rights reserved.
**
** This file is part of the KDE installer for windows
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "globalconfig.h"
#include "downloader.h"

#include <QFileInfo>
#include <QBuffer>
#include <QMessageBox>

GlobalConfig::GlobalConfig(const QString &url, Downloader &downloader)
{
    QFileInfo cfi("config.txt");
    qDebug() << "download global configuration file";
    // FIXME uses version related config file to have more room for format changes
    downloader.start(url,cfi.fileName());

    qDebug() << "parsing remote configuration file";
    int ret = parseFromFile("config.txt");
    QFileInfo fi("config-local.txt");
    if (fi.exists()) 
    {
        ret = parseFromFile(fi.absoluteFilePath());
        qDebug() << "parsing local configuration file";
    }
}

GlobalConfig::~GlobalConfig()
{
    for (QList<Site*>::iterator s = m_sites.begin(); s != m_sites.end(); s++)
        delete (*s);
    for (QList<Package*>::iterator s = m_packages.begin(); s != m_packages.end(); s++)
        delete (*s);
}

bool GlobalConfig::parseFromFile(const QString &_fileName)
{
    QString fileName(_fileName);
    QFileInfo fi(fileName);
    if(!fi.isAbsolute())
        fileName = fi.absoluteFilePath();
    QFile configFile(fileName);
    if (!configFile.exists())
    {
#ifdef USE_GUI
        QMessageBox::warning(NULL, QString("File not found"), QString("Can't find %1").arg(fileName));
#endif

        return false;
    }

    configFile.open(QIODevice::ReadOnly | QIODevice::Text);

    return parse(&configFile);
}

bool GlobalConfig::parseFromByteArray(const QByteArray &_ba)
{
    QByteArray ba(_ba);
    QBuffer buf(&ba);

    if (!buf.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    return parse(&buf);
}

bool GlobalConfig::parse(QIODevice *ioDev)
{
    bool inSite = false;
    bool inPackage = false;
    Site *site;
    Package *pkg;

    while (!ioDev->atEnd())
    {
        QByteArray line = ioDev->readLine().replace("\x0a",""); // support unix format file
        if (line.startsWith(';'))
            continue;
        else if (line.size() < 2)
        { 
           if (inPackage)
           {
               pkg->dump();
               inPackage=false;
           }
           else if (inSite)
             inSite=false;
           continue;
        }           
        else if (line.startsWith('@'))
        {
            // TODO: encoding of config file! Currently: ascii
            QString l = line;
            QStringList cmd = l.split(' ');
            if (cmd[0] == "@format")
                ;
            else if(inPackage)
            {
                if(cmd[0] == "@version")
                    pkg->setVersion(cmd[1]);
                else if(cmd[0] == "@url-bin")
                    pkg->add(QString(), cmd[1],Package::BIN);
                else if(cmd[0] == "@url-lib")
                    pkg->add(QString(), cmd[1],Package::LIB);
                else if(cmd[0] == "@url-doc")
                    pkg->add(QString(), cmd[1],Package::DOC);
                else if(cmd[0] == "@url-src")
                    pkg->add(QString(), cmd[1],Package::SRC);
                else if(cmd[0] == "@require")
                {
                    cmd.removeFirst();
                    pkg->addDeps(cmd);
                }
                else if(cmd[0] == "@category")
                    pkg->setCategory(cmd[1]);
                else if(cmd[0] == "@relocate")
                    pkg->addPathRelocation(cmd[1],cmd[2]);
            }
            else if (inSite)
            {
                if(cmd[0] == "@siteurl" || cmd[0] == "@url")
                    site->setURL(cmd[1]);
                else if(cmd[0] == "@sitetype" || cmd[0] == "@type")
                    site->setType(cmd[1] == "apachemodindex" ? Site::ApacheModIndex : Site::SourceForge );
                else if(cmd[0] == "@mirrorurl")
                    site->addMirror(cmd[1]);
                else if(cmd[0] == "@deps") {
                    QString dep = cmd[1];
                    cmd.removeFirst();
                    cmd.removeFirst();
                    site->addDependencies(dep, cmd);
                }
                else if(cmd[0] == "@exclude") {
                    cmd.removeFirst();
                    site->addExcludes(cmd);
                }
            }
            else if(cmd[0] == "@site")
            {
                site = new Site;
                m_sites.append(site);
                site->setName(cmd[1]);
                inSite = true;
            }
            else if(cmd[0] == "@package") 
            {
                 pkg = new Package;
                 m_packages.append(pkg);
                 pkg->setName(cmd[1]);
                 inPackage=true;
            }
        }
    }
    return true;
}
