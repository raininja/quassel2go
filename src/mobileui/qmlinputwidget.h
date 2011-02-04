#ifndef QMLINPUTWIDGET_H
#define QMLINPUTWIDGET_H

#include <QGraphicsProxyWidget>
#include <QWidget>

class QmlInputWidget: public QGraphicsProxyWidget
{
    Q_OBJECT

  Q_PROPERTY(QWidget *widget READ widget WRITE setWidget)

public:
    explicit QmlInputWidget(QWidget *parent = 0);

  static void setEmbeddedWidget(QWidget *widget);

public slots:

signals:

private:
  static QWidget *_widget;
};

#endif // QMLINPUTWIDGET_H
