#include "qmlinputwidget.h"
#include <QDebug>

QWidget *QmlInputWidget::_widget = 0;

QmlInputWidget::QmlInputWidget(QWidget *parent) :
    QGraphicsProxyWidget()
{
  if(_widget) {
    //_widget->setAttribute(Qt::WA_NoSystemBackground);
    setWidget(_widget);
  } else {
    qWarning() << "QmlChatView needs to be initialized first with ::setBufferWidget()";
    setWidget(new QWidget());
  }
}

void QmlInputWidget::setEmbeddedWidget(QWidget *widget)
{
  _widget = widget;
}
