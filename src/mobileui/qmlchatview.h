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
  Q_PROPERTY(bool focus READ hasFocus WRITE setWidgetFocus NOTIFY widgetFocusChanged)

public:
    explicit QmlChatView(QWidget *parent = 0);

  static void setBufferWidget(QWidget *widget);

  QmlScrollModel *scrollModel() const;

public slots:
  void requestBacklog();
  void setWidgetFocus(bool focus);

signals:
  void widgetChanged();

  void scrollModelChanged();

  void widgetFocusChanged();

protected:
  virtual void focusInEvent ( QFocusEvent * event );
  virtual bool focusNextPrevChild ( bool next );
  virtual void focusOutEvent ( QFocusEvent * event );

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
