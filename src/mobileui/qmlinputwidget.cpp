#include "qmlinputwidget.h"
#include <QDebug>

QWidget *QmlInputWidget::_widget = 0;

QmlInputWidget::QmlInputWidget(QWidget *parent) :
    QGraphicsProxyWidget()
{
  setWidget(_widget);
}

void QmlInputWidget::setEmbeddedWidget(QWidget *widget)
{
  _widget = widget;
}
