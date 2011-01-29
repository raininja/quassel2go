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

#ifndef TOPICMODEL_H_
#define TOPICMODEL_H_

#include <QPointer>
#include <QObject>
#include <QAbstractItemModel>
#include <QItemSelectionModel>

class TopicModel : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString currentTopic READ currentTopic WRITE setCurrentTopic NOTIFY currentTopicChanged)
  Q_PROPERTY(bool readOnly READ isReadOnly NOTIFY readOnlyChanged)

public:
  TopicModel(QObject *parent = 0);

  const QString &currentTopic() const { return _topic; }
  inline bool isReadOnly() const { return _readonly; }

  inline QAbstractItemModel *model() { return _model; }
  void setModel(QAbstractItemModel *model);
  inline QItemSelectionModel *selectionModel() const { return _selectionModel; }
  void setSelectionModel(QItemSelectionModel *selectionModel);

public slots:
  void setCurrentTopic(const QString &topic);

signals:
  void currentTopicChanged(const QString &topic);
  void readOnlyChanged(bool readonly);


protected slots:
  virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
  virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
  void setReadOnly(const bool &readonly);
  void _updateTopic(const QModelIndex &index);
  void _changeTopic(const QString &topic);
  QPointer<QAbstractItemModel> _model;
  QPointer<QItemSelectionModel> _selectionModel;

  QString _topic;
  bool _readonly;
};


#endif
