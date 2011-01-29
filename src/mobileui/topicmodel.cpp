/***************************************************************************
 *   Copyright (C) 2005/06 by the Quassel Project                          *
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

#include <QTextDocument>  // for Qt::escape()

#include "topicmodel.h"

#include "client.h"
#include "networkmodel.h"

TopicModel::TopicModel(QObject *parent)
  : QObject(parent),
    _model(0),
    _selectionModel(0),
    _readonly(false)
{
}

void TopicModel::setModel(QAbstractItemModel *model) {
  if(_model) {
    disconnect(_model, 0, this, 0);
  }
  _model = model;
  connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
      this, SLOT(dataChanged(QModelIndex, QModelIndex)));
  connect(model, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)),
      this, SLOT(rowsAboutToBeRemoved(QModelIndex, int, int)));
  connect(model, SIGNAL(rowsInserted(QModelIndex, int, int)),
      this, SLOT(rowsInserted(QModelIndex, int, int)));
}

void TopicModel::setSelectionModel(QItemSelectionModel *selectionModel) {
  if(_selectionModel) {
    disconnect(_selectionModel, 0, this, 0);
  }
  _selectionModel = selectionModel;
  connect(selectionModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
      this, SLOT(currentChanged(QModelIndex, QModelIndex)));
  connect(selectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
      this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
}

void TopicModel::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
  Q_UNUSED(previous);
  _updateTopic(current);
}

void TopicModel::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
  QItemSelectionRange changedArea(topLeft, bottomRight);
  QModelIndex currentTopicIndex = selectionModel()->currentIndex().sibling(selectionModel()->currentIndex().row(), 1);
  if(changedArea.contains(currentTopicIndex))
    _updateTopic(selectionModel()->currentIndex());
};

void TopicModel::_updateTopic(const QModelIndex &index) {
  QString newtopic;
  bool readonly = true;

  BufferId id = index.data(NetworkModel::BufferIdRole).value<BufferId>();
  if(id.isValid()) {
    QModelIndex index0 = index.sibling(index.row(), 0);
    const Network *network = Client::network(Client::networkModel()->networkId(id));

    switch(Client::networkModel()->bufferType(id)) {
    case BufferInfo::StatusBuffer:
      if(network) {
        newtopic = QString("%1 (%2) | %3 | %4")
          .arg(Qt::escape(network->networkName()))
          .arg(Qt::escape(network->currentServer()))
          .arg(tr("Users: %1").arg(network->ircUsers().count()))
          .arg(tr("Lag: %1 msecs").arg(network->latency()));
      } else {
        newtopic = index0.data(Qt::DisplayRole).toString();
      }
      break;

    case BufferInfo::ChannelBuffer:
      newtopic = index.sibling(index.row(), 1).data().toString();
      readonly = false;
      break;

    case BufferInfo::QueryBuffer:
      {
        QString nickname = index0.data(Qt::DisplayRole).toString();
        if(network) {
          const IrcUser *user = network->ircUser(nickname);
          if(user) {
            newtopic = QString("%1%2%3 | %4@%5").arg(nickname)
              .arg(user->userModes().isEmpty() ? QString() : QString(" (+%1)").arg(user->userModes()))
              .arg(user->realName().isEmpty() ? QString() : QString(" | %1").arg(user->realName()))
              .arg(user->user())
            .arg(user->host());
          } else { // no such user
            newtopic = nickname;
          }
        } else { // no valid Network-Obj.
          newtopic = nickname;
        }
        break;
      }
    default:
      newtopic = index0.data(Qt::DisplayRole).toString();
    }
  }

  _changeTopic(newtopic);
  setReadOnly(readonly);
}

void TopicModel::setCurrentTopic(const QString &topic) {
  QModelIndex currentIdx = selectionModel()->currentIndex();
  if(currentIdx.isValid() && currentIdx.data(NetworkModel::BufferTypeRole) == BufferInfo::ChannelBuffer) {
    BufferInfo bufferInfo = currentIdx.data(NetworkModel::BufferInfoRole).value<BufferInfo>();
    if(topic.isEmpty())
      Client::userInput(bufferInfo, QString("/quote TOPIC %1 :").arg(bufferInfo.bufferName()));
    else
      Client::userInput(bufferInfo, QString("/topic %1").arg(topic));
  }

  _changeTopic(topic);
}

void TopicModel::_changeTopic(const QString &topic) {
  if(_topic == topic)
    return;
  _topic = topic;
  emit currentTopicChanged(_topic);
}

void TopicModel::setReadOnly(const bool &readonly) {
  if(_readonly == readonly)
    return;
  _readonly = readonly;
  emit readOnlyChanged(_readonly);
}


