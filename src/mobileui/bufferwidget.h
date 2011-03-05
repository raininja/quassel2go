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

#ifndef BUFFERWIDGET_H_
#define BUFFERWIDGET_H_

#include "ui_bufferwidget.h"

#include "abstractbuffercontainer.h"

class QGraphicsItem;
class ChatView;
class ChatViewSearchBar;
class ChatViewSearchController;

class BufferWidget : public AbstractBufferContainer {
  Q_OBJECT

  Q_PROPERTY(BufferId currentBuffer READ currentBuffer NOTIFY currentIdChanged)
  Q_PROPERTY(ChatView *currentChatView READ currentChatView NOTIFY currentChatViewChanged)
  Q_PROPERTY(bool searchBarVisible READ searchBarVisible NOTIFY searchBarVisibleChanged)

public:
  BufferWidget(QWidget *parent);
  ~BufferWidget();

  virtual bool eventFilter(QObject *watched, QEvent *event);

  inline ChatViewSearchBar *searchBar() const { return ui.searchBar; }
  void addActionsToMenu(QMenu *, const QPointF &pos);

  ChatView *currentChatView() const;

  bool searchBarVisible() const { return searchBar()->isVisible(); }

public slots:
  virtual void setMarkerLine(ChatView *view = 0, bool allowGoingBack = true);
  virtual void jumpToMarkerLine(ChatView *view = 0, bool requestBacklog = true);

protected:
  virtual AbstractChatView *createChatView(BufferId);
  virtual void removeChatView(BufferId);
  virtual inline bool autoMarkerLine() const { return _autoMarkerLine; }

protected slots:
  virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
  virtual void showChatView(BufferId);

private slots:
  void scrollToHighlight(QGraphicsItem *highlightItem);
  void zoomIn();
  void zoomOut();
  void zoomOriginal();

  void setAutoMarkerLine(const QVariant &);

signals:
  void currentIdChanged(BufferId id);
  void currentChatViewChanged(ChatView *view);

  void searchBarVisibleChanged();

private:
  Ui::BufferWidget ui;
  QHash<BufferId, QWidget *> _chatViews;

  ChatViewSearchController *_chatViewSearchController;

  bool _autoMarkerLine;
};

#endif
