/***************************************************************************
 *   Copyright (C) 2005-08 by the Quassel IRC Team                         *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Blank Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Blank Public License for more details.                            *
 *                                                                         *
 *   You should have received a copy of the GNU Blank Public License       *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "chatmonitorsettingspage.h"

#include "client.h"
#include "networkmodel.h"
#include "bufferviewconfig.h"
#include "buffermodel.h"
#include "bufferview.h"
#include "bufferviewfilter.h"
#include "iconloader.h"
#include "chatviewsettings.h"

#include <QVariant>

ChatMonitorSettingsPage::ChatMonitorSettingsPage(QWidget *parent)
  : SettingsPage(tr("General"), tr("Chat Monitor"), parent) {
  ui.setupUi(this);

  ui.activateBuffer->setIcon(SmallIcon("go-next"));
  ui.deactivateBuffer->setIcon(SmallIcon("go-previous"));

  // setup available buffers config (for the bufferview on the left)
  _configAvailable = new BufferViewConfig(-667, this);
  _configAvailable->setBufferViewName("tmpChatMonitorAvailableBuffers");
  _configAvailable->setSortAlphabetically(true);
  _configAvailable->setDisableDecoration(true);
  _configAvailable->setNetworkId(NetworkId());
  _configAvailable->setInitialized();

  // setup active buffers config (for the bufferview on the right)
  _configActive = new BufferViewConfig(-666, this);
  _configActive->setBufferViewName("tmpChatMonitorActiveBuffers");
  _configActive->setSortAlphabetically(true);
  _configActive->setDisableDecoration(true);
  _configActive->setNetworkId(NetworkId());
  _configActive->setInitialized();

  // fill combobox with operation modes
  ui.operationMode->addItem(tr("Opt In"), ChatViewSettings::OptIn);
  ui.operationMode->addItem(tr("Opt Out"), ChatViewSettings::OptOut);

  // connect slots
  connect(ui.operationMode, SIGNAL(currentIndexChanged(int)), this, SLOT(switchOperationMode(int)));
}

bool ChatMonitorSettingsPage::hasDefaults() const {
  return true;
}

void ChatMonitorSettingsPage::defaults() {
  settings["OperationMode"] = ChatViewSettings::OptOut;
  settings["ShowHighlights"] = false;
  settings["Buffers"] = QVariant();
  settings["Default"] = true;
  load();
  widgetHasChanged();
}

void ChatMonitorSettingsPage::load() {
  if(settings.contains("Default"))
    settings.remove("Default");
  else
    loadSettings();

  ui.operationMode->setCurrentIndex(settings["OperationMode"].toInt() - 1);
  ui.showHighlights->setChecked(settings["ShowHighlights"].toBool());

  // get all available buffer Ids
  QList<BufferId> allBufferIds = Client::networkModel()->allBufferIds();

  if(!settings["Buffers"].toList().isEmpty()) {
    QList<BufferId> bufferIdsFromConfig;
    // remove all active buffers from the available config
    foreach(QVariant v, settings["Buffers"].toList()) {
      bufferIdsFromConfig << v.value<BufferId>();
      allBufferIds.removeAll(v.value<BufferId>());
    }
    qSort(bufferIdsFromConfig.begin(), bufferIdsFromConfig.end(), bufferIdLessThan);
    _configActive->initSetBufferList(bufferIdsFromConfig);
  }
  ui.activeBuffers->setFilteredModel(Client::bufferModel(), _configActive);

  qSort(allBufferIds.begin(), allBufferIds.end(), bufferIdLessThan);
  _configAvailable->initSetBufferList(allBufferIds);
  ui.availableBuffers->setFilteredModel(Client::bufferModel(), _configAvailable);

  setChangedState(false);
}

void ChatMonitorSettingsPage::loadSettings() {
  ChatViewSettings chatViewSettings("ChatMonitor");
  settings["OperationMode"] = static_cast<ChatViewSettings::OperationMode>(chatViewSettings.value("OperationMode", QVariant()).toInt());

  // Load default behavior if no or invalid settings found
  if(settings["OperationMode"] == ChatViewSettings::InvalidMode) {
    switchOperationMode(ui.operationMode->findData(ChatViewSettings::OptOut));
    settings["OperationMode"] == ChatViewSettings::OptOut;
  }
  settings["ShowHighlights"] = chatViewSettings.value("ShowHighlights", false);
  settings["Buffers"] = chatViewSettings.value("Buffers", QVariantList());
}

void ChatMonitorSettingsPage::save() {
  ChatViewSettings chatViewSettings("ChatMonitor");
  // save operation mode
  chatViewSettings.setValue("OperationMode", settings["OperationMode"]);
  chatViewSettings.setValue("ShowHighlights", settings["ShowHighlights"]);

  // save list of active buffers
  QVariantList saveableBufferIdList;
  foreach(BufferId id, _configActive->bufferList()) {
    saveableBufferIdList << QVariant::fromValue<BufferId>(id);
  }

  chatViewSettings.setValue("Buffers", saveableBufferIdList);
  load();
  setChangedState(false);
}

void ChatMonitorSettingsPage::widgetHasChanged() {
  bool changed = testHasChanged();
  if(changed != hasChanged()) setChangedState(changed);
}

bool ChatMonitorSettingsPage::testHasChanged() {
  if(settings["OperationMode"] != ui.operationMode->itemData(ui.operationMode->currentIndex()))
    return true;

  if(_configActive->bufferList().count() != settings["Buffers"].toList().count())
    return true;

  QSet<BufferId> uiBufs = _configActive->bufferList().toSet();
  QSet<BufferId> settingsBufs;
  foreach(QVariant v, settings["Buffers"].toList())
    settingsBufs << v.value<BufferId>();
  if(uiBufs != settingsBufs)
    return true;

  return false;
}

//TODO: - support drag 'n drop
//      - adding of complete networks(?)

/*
  toggleBuffers takes each a bufferView and its config for "input" and "output".
  Any selected item will be moved over from the input to the output bufferview.
*/
void ChatMonitorSettingsPage::toggleBuffers(BufferView *inView, BufferViewConfig *inCfg, BufferView *outView, BufferViewConfig *outCfg) {

  // Fill QMap with selected items ordered by selection row
  QMap<int, QList<BufferId> > selectedBuffers;
  foreach(QModelIndex index, inView->selectionModel()->selectedIndexes()) {
    BufferId inBufferId = index.data(NetworkModel::BufferIdRole).value<BufferId>();
    if(index.data(NetworkModel::ItemTypeRole) == NetworkModel::NetworkItemType) {
      // TODO:
      //  If item is a network: move over all children and skip other selected items of this node
    }
    else if(index.data(NetworkModel::ItemTypeRole) == NetworkModel::BufferItemType) {
      selectedBuffers[index.parent().row()] << inBufferId;
    }
  }

  // clear selection to be able to remove the bufferIds without errors
  inView->selectionModel()->clearSelection();

  /*
    Invalidate the BufferViewFilters' configs to get constant add/remove times
    even for huge lists.
    This can probably be removed whenever BufferViewConfig::bulkAdd or something
    like that is available.
  */
  qobject_cast<BufferViewFilter *>(outView->model())->setConfig(0);
  qobject_cast<BufferViewFilter *>(inView->model())->setConfig(0);

  // actually move the ids
  foreach (QList<BufferId> list, selectedBuffers) {
    foreach (BufferId buffer, list) {
      outCfg->addBuffer(buffer,0);
      inCfg->removeBuffer(buffer);
    }
  }

  outView->setFilteredModel(Client::bufferModel(), outCfg);
  inView->setFilteredModel(Client::bufferModel(), inCfg);

  widgetHasChanged();
}

void ChatMonitorSettingsPage::on_activateBuffer_clicked() {
  if (ui.availableBuffers->currentIndex().isValid() && ui.availableBuffers->selectionModel()->hasSelection()) {
    toggleBuffers(ui.availableBuffers, _configAvailable, ui.activeBuffers, _configActive);
    widgetHasChanged();
  }
}

void ChatMonitorSettingsPage::on_deactivateBuffer_clicked() {
  if (ui.activeBuffers->currentIndex().isValid() && ui.activeBuffers->selectionModel()->hasSelection()) {
    toggleBuffers(ui.activeBuffers, _configActive, ui.availableBuffers, _configAvailable);
    widgetHasChanged();
  }
}

void ChatMonitorSettingsPage::on_showHighlights_toggled(bool state)
{
  settings["ShowHighlights"] = state;
  widgetHasChanged();
}

/*
  switchOperationMode gets called on combobox signal currentIndexChanged.
  modeIndex is the row id in combobox itemlist
*/
void ChatMonitorSettingsPage::switchOperationMode(int modeIndex) {
  ChatViewSettings::OperationMode newMode = static_cast<ChatViewSettings::OperationMode>(ui.operationMode->itemData(modeIndex).toInt());

  if(newMode == ChatViewSettings::OptIn) {
    ui.labelActiveBuffers->setText(tr("Show:"));
  }
  else if(newMode == ChatViewSettings::OptOut) {
    ui.labelActiveBuffers->setText(tr("Ignore:"));
  }
  widgetHasChanged();
}