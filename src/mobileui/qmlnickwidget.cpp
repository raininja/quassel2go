#include <QComboBox>

#include "qmlnickwidget.h"
#include "inputnickpresenter.h"
#include "client.h"

QmlNickWidget::QmlNickWidget(QWidget *parent) :
    QGraphicsProxyWidget()
{
  _nick = new QComboBox(parent);
  _nick->setAttribute(Qt::WA_NoSystemBackground, true);
  setWidget(_nick);
  _presenter = new InputNickPresenter(_nick, parent);

  _presenter->setModel(Client::bufferModel());
  _presenter->setSelectionModel(Client::bufferModel()->standardSelectionModel());
}


void QmlNickWidget::setWidgetFocus(bool focus)
{
  if(focus) {
    setFocus();
    _nick->setFocus();
  } else {
    clearFocus();
    _nick->clearFocus();
  }
}

void QmlNickWidget::focusInEvent ( QFocusEvent * event )
{
  emit widgetFocusChanged();
  QGraphicsProxyWidget::focusInEvent(event);
}

bool QmlNickWidget::focusNextPrevChild ( bool next )
{
  emit widgetFocusChanged();
  return QGraphicsProxyWidget::focusNextPrevChild(next);
}

void QmlNickWidget::focusOutEvent ( QFocusEvent * event )
{
  emit widgetFocusChanged();
  QGraphicsProxyWidget::focusOutEvent(event);
}
