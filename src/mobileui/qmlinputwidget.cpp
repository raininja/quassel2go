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

