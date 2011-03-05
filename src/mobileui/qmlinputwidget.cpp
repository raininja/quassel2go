#include "qmlinputwidget.h"
#include "inputwidget.h"
#include "inputpresenter.h"
#include "multilineedit.h"

#include "client.h"

#include <QDebug>

//QWidget *QmlInputWidget::_widget = 0;

QmlInputWidget::QmlInputWidget(QWidget *parent) :
    QGraphicsProxyWidget()
{
  //setFocusPolicy(Qt::StrongFocus);
  setFocus(Qt::OtherFocusReason);

  _input = new MultiLineEdit(parent);
  _input->setAttribute(Qt::WA_NoSystemBackground, true);
  _input->setFocus();
  setWidget(_input);
  _presenter = new InputPresenter(_input, parent);

  _presenter->setModel(Client::bufferModel());
  _presenter->setSelectionModel(Client::bufferModel()->standardSelectionModel());

  connect(_input, SIGNAL(textChanged()), this, SIGNAL(heightHintChanged()));
}

int QmlInputWidget::heightHint() const
{
  return _input->sizeHint().height();
}

void QmlInputWidget::completeNick()
{
  // Attention: This is a Hack/Workaround!
  // Turning the focus on/off fixes a problem with a bug related to predictive
  // text (the text fragment proposed by the dictionary stays visible even
  // after the auto completion. makes things look buggy)
  bool oldFocus = hasFocus();
  setWidgetFocus(false);

  _presenter->completeNick();

  if(oldFocus)
    setWidgetFocus(true);
}

void QmlInputWidget::setWidgetFocus(bool focus)
{
  if(focus) {
    setFocus();
    _input->setFocus();
  } else {
    clearFocus();
    _input->clearFocus();
  }
}

void QmlInputWidget::focusInEvent ( QFocusEvent * event )
{
  emit widgetFocusChanged();
  QGraphicsProxyWidget::focusInEvent(event);
}

bool QmlInputWidget::focusNextPrevChild ( bool next )
{
  emit widgetFocusChanged();
  return QGraphicsProxyWidget::focusNextPrevChild(next);
}

void QmlInputWidget::focusOutEvent ( QFocusEvent * event )
{
  emit widgetFocusChanged();
  QGraphicsProxyWidget::focusOutEvent(event);
}
