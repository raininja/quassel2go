#ifndef QMLNICKWIDGET_H
#define QMLNICKWIDGET_H

#include <QGraphicsProxyWidget>

class QmlNickWidget : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    explicit QmlNickWidget(QWidget *parent = 0);

signals:

private:
  class QComboBox *_nick;
  class InputNickPresenter *_presenter;
};

#endif // QMLNICKWIDGET_H
