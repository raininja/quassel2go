#ifndef QMLCHATVIEW_H
#define QMLCHATVIEW_H

#include <QGraphicsProxyWidget>
#include "chatview.h"

class QmlScrollModel;

class QmlChatView : public QGraphicsProxyWidget
{
    Q_OBJECT

  Q_PROPERTY(QWidget *widget READ widget WRITE setWidget NOTIFY widgetChanged)

  Q_PROPERTY(QmlScrollModel *scrollModel READ scrollModel NOTIFY scrollModelChanged);

public:
    explicit QmlChatView(QWidget *parent = 0);

  static void setBufferWidget(QWidget *widget);

  QmlScrollModel *scrollModel() const;

public slots:
  void requestBacklog();

signals:
  void widgetChanged();

  void scrollModelChanged();

protected slots:
  void currentChatViewChanged(ChatView *view);

private:
  //BufferId _id;
  //QWidget *_par;

  static QWidget *_bufferWidget;

  ChatView *_currentView;

  QmlScrollModel *_scrollModel;
};

#endif // QMLCHATVIEW_H
