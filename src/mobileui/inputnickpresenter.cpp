/***************************************************************************
 *   Copyright (C) 2005-2010 by the Quassel Project                        *
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


#include "inputnickpresenter.h"
#include "inputwidget.h"

#include "action.h"
#include "actioncollection.h"
#include "bufferview.h"
#include "client.h"
#include "iconloader.h"
#include "ircuser.h"
#include "networkmodel.h"
#include "qtui.h"
#include "qtuisettings.h"
#include "tabcompleter.h"
#include <QPainter>

const int leftMargin = 3;


InputNickPresenter::InputNickPresenter(QComboBox *ownNick, QObject *parent) :
    AbstractItemPresenter(parent),
  _ownNick(ownNick),
    _networkId(0)
{
  connect(_ownNick, SIGNAL(activated(QString)), this, SLOT(changeNick(QString)));

  _ownNick->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  _ownNick->installEventFilter(new MouseWheelFilter(this));

  UiSettings s("InputWidget");

  s.notify("ShowNickSelector", this, SLOT(setShowNickSelector(QVariant)));
  setShowNickSelector(s.value("ShowNickSelector", true));
}

InputNickPresenter::~InputNickPresenter() {
}

void InputNickPresenter::setShowNickSelector(const QVariant &v) {
  _ownNick->setVisible(v.toBool());
}

void InputNickPresenter::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
  BufferId currentBufferId = current.data(NetworkModel::BufferIdRole).value<BufferId>();
  BufferId previousBufferId = previous.data(NetworkModel::BufferIdRole).value<BufferId>();

  NetworkId networkId = current.data(NetworkModel::NetworkIdRole).value<NetworkId>();
  if(networkId == _networkId)
    return;

  setNetwork(networkId);
  updateNickSelector();
  updateEnabledState();
}

void InputNickPresenter::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
  QItemSelectionRange changedArea(topLeft, bottomRight);
  if(changedArea.contains(selectionModel()->currentIndex())) {
    updateEnabledState();
  }
};

void InputNickPresenter::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end) {
  NetworkId networkId;
  QModelIndex child;
  for(int row = start; row <= end; row++) {
    child = model()->index(row, 0, parent);
    if(NetworkModel::NetworkItemType != child.data(NetworkModel::ItemTypeRole).toInt())
      continue;
    networkId = child.data(NetworkModel::NetworkIdRole).value<NetworkId>();
    if(networkId == _networkId) {
      setNetwork(0);
      updateNickSelector();
      return;
    }
  }
}


void InputNickPresenter::updateEnabledState() {
// FIXME: Find a visualization for this that does not disable the widget!
//        Disabling kills global action shortcuts, plus users sometimes need/want to enter text
//        even in inactive channels.
#if 0
  QModelIndex currentIndex = selectionModel()->currentIndex();

  const Network *net = Client::networkModel()->networkByIndex(currentIndex);
  bool enabled = false;
  if(net) {
    // disable inputline if it's a channelbuffer we parted from or...
    enabled = (currentIndex.data(NetworkModel::ItemActiveRole).value<bool>() || (currentIndex.data(NetworkModel::BufferTypeRole).toInt() != BufferInfo::ChannelBuffer));
    // ... if we're not connected to the network at all
    enabled &= net->isConnected();
  }
#endif
}

const Network *InputNickPresenter::currentNetwork() const {
  return Client::network(_networkId);
}

BufferInfo InputNickPresenter::currentBufferInfo() const {
  return selectionModel()->currentIndex().data(NetworkModel::BufferInfoRole).value<BufferInfo>();
};

void InputNickPresenter::setNetwork(NetworkId networkId) {
  if(_networkId == networkId)
    return;

  const Network *previousNet = Client::network(_networkId);
  if(previousNet) {
    disconnect(previousNet, 0, this, 0);
    if(previousNet->me())
      disconnect(previousNet->me(), 0, this, 0);
  }

  _networkId = networkId;

  const Network *network = Client::network(networkId);
  if(network) {
    connect(network, SIGNAL(identitySet(IdentityId)), this, SLOT(setIdentity(IdentityId)));
    connectMyIrcUser();
    setIdentity(network->identity());
  } else {
    setIdentity(0);
    _networkId = 0;
  }
}

void InputNickPresenter::connectMyIrcUser() {
  const Network *network = currentNetwork();
  if(network->me()) {
    connect(network->me(), SIGNAL(nickSet(const QString &)), this, SLOT(updateNickSelector()));
    connect(network->me(), SIGNAL(userModesSet(QString)), this, SLOT(updateNickSelector()));
    connect(network->me(), SIGNAL(userModesAdded(QString)), this, SLOT(updateNickSelector()));
    connect(network->me(), SIGNAL(userModesRemoved(QString)), this, SLOT(updateNickSelector()));
    connect(network->me(), SIGNAL(awaySet(bool)), this, SLOT(updateNickSelector()));
    disconnect(network, SIGNAL(myNickSet(const QString &)), this, SLOT(connectMyIrcUser()));
    updateNickSelector();
  } else {
    connect(network, SIGNAL(myNickSet(const QString &)), this, SLOT(connectMyIrcUser()));
  }
}

void InputNickPresenter::setIdentity(IdentityId identityId) {
  if(_identityId == identityId)
    return;

  const Identity *previousIdentity = Client::identity(_identityId);
  if(previousIdentity)
    disconnect(previousIdentity, 0, this, 0);

  _identityId = identityId;

  const Identity *identity = Client::identity(identityId);
  if(identity) {
    connect(identity, SIGNAL(nicksSet(QStringList)), this, SLOT(updateNickSelector()));
  } else {
    _identityId = 0;
  }
  updateNickSelector();
}

void InputNickPresenter::updateNickSelector() const {
  _ownNick->clear();

  const Network *net = currentNetwork();
  if(!net)
    return;

  const Identity *identity = Client::identity(net->identity());
  if(!identity) {
    qWarning() << "InputNickPresenter::updateNickSelector(): can't find Identity for Network" << net->networkId() << "IdentityId:" << net->identity();
    return;
  }

  int nickIdx;
  QStringList nicks = identity->nicks();
  if((nickIdx = nicks.indexOf(net->myNick())) == -1) {
    nicks.prepend(net->myNick());
    nickIdx = 0;
  }

  if(nicks.isEmpty())
    return;

  IrcUser *me = net->me();
  if(me) {
    nicks[nickIdx] = net->myNick();
    if(!me->userModes().isEmpty())
      nicks[nickIdx] += QString(" (+%1)").arg(me->userModes());
  }

  _ownNick->addItems(nicks);

  if(me && me->isAway())
    _ownNick->setItemData(nickIdx, SmallIcon("user-away"), Qt::DecorationRole);

  _ownNick->setCurrentIndex(nickIdx);
}

void InputNickPresenter::changeNick(const QString &newNick) const {
  const Network *net = currentNetwork();
  if(!net || net->isMyNick(newNick))
    return;

  // we reset the nick selecter as we have no confirmation yet, that this will succeed.
  // if the action succeeds it will be properly updated anyways.
  updateNickSelector();
  Client::userInput(BufferInfo::fakeStatusBuffer(net->networkId()), QString("/NICK %1").arg(newNick));
}
