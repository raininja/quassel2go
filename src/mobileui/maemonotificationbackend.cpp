/***************************************************************************
*   Copyright (C) 2005-09 by the Quassel Project                          *
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

#include "maemonotificationbackend.h"
#include "maemo5notification.h"

#include <QtGui>
#include <QDebug>

#include "clientsettings.h"
#include "iconloader.h"
#include "mainwin.h"
#include "qtui.h"

MaemoNotificationBackend::MaemoNotificationBackend(QObject *parent)
  : AbstractNotificationBackend(parent)
{
  NotificationSettings notificationSettings;
  _enabled = notificationSettings.value("MaemoNotification/Enabled", true).toBool();
  _timeout = notificationSettings.value("MaemoNotification/Timeout", 0).toInt();

  notificationSettings.notify("MaemoNotification/Enabled", this, SLOT(enabledChanged(const QVariant &)));
  notificationSettings.notify("MaemoNotification/Timeout", this, SLOT(timeoutChanged(const QVariant &)));
}

void MaemoNotificationBackend::notify(const Notification &notification) {
  if(!_enabled)
    return;

  if(! (notification.type == Highlight || notification.type == PrivMsg)) {
    return;
  }

  QString text = tr("%1: %2").arg(notification.sender).arg(notification.message);
  if(text.size() > 64) {
    text.resize(59);
    text += "[...]";
  }
  if(text.size() > 0)
    Maemo5Notification::information(text, _timeout);
}

void MaemoNotificationBackend::close(uint notificationId) {
  Q_UNUSED(notificationId);
}

void MaemoNotificationBackend::enabledChanged(const QVariant &v) {
  _enabled = v.toBool();
}

void MaemoNotificationBackend::timeoutChanged(const QVariant &v) {
  _timeout = v.toInt();
}

SettingsPage *MaemoNotificationBackend::createConfigWidget() const {
  return new ConfigWidget();
}

/***************************************************************************/

MaemoNotificationBackend::ConfigWidget::ConfigWidget(QWidget *parent) : SettingsPage("Internal", "MaemoNotification", parent) {
  QHBoxLayout *layout = new QHBoxLayout(this);

  layout->addWidget(enabledBox = new QCheckBox(tr("Display notifications:"), this));

  enabledBox->setIcon(SmallIcon("flag-blue"));
  enabledBox->setEnabled(true);

  timeoutBox = new QSpinBox(this);
  timeoutBox->setMinimum(1);
  timeoutBox->setMaximum(60);
  timeoutBox->setSuffix(tr(" seconds"));
  layout->addWidget(timeoutBox);
  layout->addStretch(20);

  connect(enabledBox, SIGNAL(toggled(bool)), SLOT(widgetChanged()));
  connect(enabledBox, SIGNAL(toggled(bool)), timeoutBox, SLOT(setEnabled(bool)));
  connect(timeoutBox, SIGNAL(valueChanged(int)), SLOT(widgetChanged()));
}

void MaemoNotificationBackend::ConfigWidget::widgetChanged() {
  bool changed = (enabled != enabledBox->isChecked() || timeout/1000 != timeoutBox->value());
  if(changed != hasChanged()) setChangedState(changed);
}

bool MaemoNotificationBackend::ConfigWidget::hasDefaults() const {
  return true;
}

void MaemoNotificationBackend::ConfigWidget::defaults() {
  enabledBox->setChecked(true);
  timeoutBox->setValue(0);
  widgetChanged();
}

void MaemoNotificationBackend::ConfigWidget::load() {
  NotificationSettings s;
  enabled = s.value("MaemoNotification/Enabled", true).toBool();
  timeout = s.value("MaemoNotification/Timeout", 3).toInt();

  enabledBox->setChecked(enabled);
  timeoutBox->setValue(timeout/1000);

  setChangedState(false);
}

void MaemoNotificationBackend::ConfigWidget::save() {
  NotificationSettings s;
  s.setValue("MaemoNotification/Enabled", enabledBox->isChecked());
  s.setValue("MaemoNotification/Timeout", timeoutBox->value() * 1000);
  load();
}
