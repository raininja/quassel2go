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
