/****************************************************************************
**
** Copyright (C) 2008 Ralf Habacker <ralf.habacker@freenet.de> 
** All rights reserved.
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

#include "finishpage.h"

#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>

FinishPage::FinishPage()
{
    setTitle(tr("Installation/Update Finished"));
    setSubTitle(tr(" "));
    
    label = new QLabel;
    label2 = new QLabel;

    label->setOpenExternalLinks (true);
    label->setWordWrap(true);
    label2->setOpenExternalLinks (true);
    label2->setWordWrap(true);
    
    QLabel *label3 = new QLabel("");
    runSystemSettingsBox = new QCheckBox("run system settings after exit");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(label2);
    layout->addWidget(label3);
    layout->addWidget(runSystemSettingsBox);
    layout->addStretch(1);
    setLayout(layout);
}

void FinishPage::initializePage()
{
    QString funText;
    if (engine->installedPackages() > 0)
    {
        setTitle(tr("Installation/Update Finished"));

        label->setText(tr(
             "<p>Now you should be able to run KDE applications. In the start menu you will find entries for the installed kde applications.</p>"
            ));

        funText = tr("<p>Have fun using KDE on windows.</p>");
    }
    else if (engine->removedPackages() > 0)
    {
        setTitle(tr("Package removal Finished"));
        label->setVisible(false);
    }
    else if (engine->installedPackages() == 0)
    {
        label->setText(tr(
             "<p>Thank you for using KDE on windows.</p>"
            ));
    }

    label2->setText(tr(
         "<p>If you <ul>"
         "<li>like to see the KDE on Windows project web site see <a href=\"http://windows.kde.org\">http://windows.kde.org</a></li>"
         "<li>like to get community support for this installer and/or running KDE applications please contact "
            "the <a href=\"http://mail.kde.org/mailman/listinfo/kde-windows\">kde-windows@kde.org</a> mailing list.</li>"
         "<li>like to contribute time and/or money to this project contact the above mentioned list."
            "There are always interesting projects where you can join.</li>"
         "</ul></p>"
         ) + funText + tr(
         "<p> </p>"
         "<p><b>The KDE on Windows team</b></p>"
         "<p>&nbsp;</p>"
         "<p>&nbsp;</p>"
         "<p>This software is designed to provide a powerfull and flexible network based install system using a modern gui toolkit.</p>"
         ));

    setFinalPage(true);
    //wizard()->setOption(QWizard::NoCancelButton,true);
    wizard()->button(QWizard::CancelButton)->setEnabled(false);
#ifdef HAVE_RETRY_BUTTON
    wizard()->setOption(QWizard::HaveCustomButton3, false);
    wizard()->button(QWizard::CustomButton3)->show();
#endif
    wizard()->setOption(QWizard::NoBackButtonOnLastPage,true);
    Settings::instance().setFirstRun(false);
    runSystemSettingsBox->setVisible(QFile::exists(engine->root() + "/bin/systemsettings.exe"));
}

bool FinishPage::isComplete()
{   
    return true;
}

bool FinishPage::validatePage()
{    
    if (runSystemSettingsBox->isChecked())
        QProcess::startDetached( engine->root() + "/bin/systemsettings.exe",QStringList(), engine->root() + "/bin");
    return true;
}

