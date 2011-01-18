/***************************************************************************
 *   Copyright (C) 2009 by the Quassel Project                             *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3.                                           *
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

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>

#include "coreconnectdlg.h"

#include "iconloader.h"
#include "clientsettings.h"
#include "coreaccountsettingspage.h"

CoreConnectDlg::CoreConnectDlg(QWidget *parent) : QDialog(parent) {
  QScrollArea *scrollArea = new QScrollArea(this);

  _settingsPage = new CoreAccountSettingsPage(scrollArea);
  _settingsPage->setStandAlone(true);
  _settingsPage->load();

  scrollArea->setWidget(_settingsPage);
  scrollArea->setWidgetResizable(true);

  CoreAccountSettings s;
  AccountId lastAccount = s.lastAccount();
  if(lastAccount.isValid())
    _settingsPage->setSelectedAccount(lastAccount);

  setWindowTitle(tr("Connect to Core"));
  setWindowIcon(SmallIcon("network-disconnect"));


  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(scrollArea);
  layout->setStretchFactor(scrollArea, 1);

  QVBoxLayout *layout2 = new QVBoxLayout(this);
  layout->addLayout(layout2);
  layout->setStretchFactor(layout2, 0);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
  buttonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
  layout2->addStretch();
  layout2->addWidget(buttonBox);

  connect(_settingsPage, SIGNAL(connectToCore(AccountId)), SLOT(accept()));
  connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
}

AccountId CoreConnectDlg::selectedAccount() const {
  return _settingsPage->selectedAccount();
}

void CoreConnectDlg::accept() {
  _settingsPage->save();
  QDialog::accept();
}

/******** CoreConnectAuthDlg ****************************************************************/

CoreConnectAuthDlg::CoreConnectAuthDlg(CoreAccount *account, QWidget *parent)
  : QDialog(parent),
  _account(account)
{
  ui.setupUi(this);

  connect(ui.user, SIGNAL(textChanged(QString)), SLOT(setButtonStates()));
  connect(ui.password, SIGNAL(textChanged(QString)), SLOT(setButtonStates()));

  ui.label->setText(tr("Please enter your credentials for %1:").arg(account->accountName()));
  ui.user->setText(account->user());
  ui.password->setText(account->password());
  ui.rememberPasswd->setChecked(account->storePassword());

  if(ui.user->text().isEmpty())
    ui.user->setFocus();
  else
    ui.password->setFocus();
}

void CoreConnectAuthDlg::accept() {
  _account->setUser(ui.user->text());
  _account->setPassword(ui.password->text());
  _account->setStorePassword(ui.rememberPasswd->isChecked());

  QDialog::accept();
}

void CoreConnectAuthDlg::setButtonStates() {
  bool valid = !(ui.user->text().isEmpty() || ui.password->text().isEmpty());
  ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
