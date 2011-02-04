#ifndef QMLCHATVIEW_H
#define QMLCHATVIEW_H

#include <QGraphicsProxyWidget>
#include "chatview.h"

class QmlScrollModel;

class QmlChatView : public QGraphicsProxyWidget
{
    Q_OBJECT

  Q_PROPERTY(QWidget *widget READ widget WRITE setWidget NOTIFY widgetChanged)

  //Q_PROPERTY(BufferId bufferId READ bufferId WRITE setBufferId)


  Q_PROPERTY(bool scrollbarMoving READ scrollbarMoving NOTIFY scrollMovingChanged)
  Q_PROPERTY(int scrollbarPos READ scrollbarPos NOTIFY scrollChanged)
  Q_PROPERTY(int scrollbarHeight READ scrollbarHeight NOTIFY scrollChanged)

  Q_PROPERTY(int contentsHeight READ contentsHeight NOTIFY scrollChanged)
  Q_PROPERTY(qreal pos READ pos WRITE setPos)

  Q_PROPERTY(QmlScrollModel *scrollModel READ scrollModel NOTIFY scrollModelChanged);

public:
    explicit QmlChatView(QWidget *parent = 0);

  //BufferId bufferId() const { return _id; };

  static void setBufferWidget(QWidget *widget);

  QmlScrollModel *scrollModel() const;

  qreal pos() const { return 0; }
  void setPos(qreal pos);

  bool scrollbarMoving() const;
  int scrollbarPos() const;
  int scrollbarHeight() const;
  int contentsHeight() const;

public slots:
  void requestBacklog();
  //void setBufferId(BufferId id);

signals:
  void widgetChanged();

  void scrollChanged();
  void scrollMovingChanged();

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
