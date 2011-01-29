#ifndef QMLCHATVIEW_H
#define QMLCHATVIEW_H

#include <QGraphicsProxyWidget>
#include "chatview.h"

class QmlChatView : public QGraphicsProxyWidget
{
    Q_OBJECT

  Q_PROPERTY(QWidget *widget READ widget WRITE setWidget)

  //Q_PROPERTY(BufferId bufferId READ bufferId WRITE setBufferId)
public:
    explicit QmlChatView(QWidget *parent = 0);

  //BufferId bufferId() const { return _id; };

  static void setBufferWidget(QWidget *widget);

public slots:
  //void setBufferId(BufferId id);

signals:

private:
  //BufferId _id;
  //QWidget *_par;

  static QWidget *_bufferWidget;
};

#endif // QMLCHATVIEW_H
