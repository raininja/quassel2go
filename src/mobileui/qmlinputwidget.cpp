#include "qmlinputwidget.h"
#include "inputwidget.h"
#include <QDebug>

QWidget *QmlInputWidget::_widget = 0;

QmlInputWidget::QmlInputWidget(QWidget *parent) :
    QGraphicsProxyWidget()
{
  //setFocusPolicy(Qt::StrongFocus);
  setFocus(Qt::OtherFocusReason);

  if(_widget) {
    //_widget->setAttribute(Qt::WA_NoSystemBackground);

    InputWidget* iw = qobject_cast<InputWidget*>(_widget);
    if(iw) {
      iw->inputLine()->setFocus();
    }

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
