#ifndef QMLINPUTWIDGET_H
#define QMLINPUTWIDGET_H

#include <QGraphicsProxyWidget>
#include <QWidget>

class QmlInputWidget: public QGraphicsProxyWidget
{
    Q_OBJECT

  Q_PROPERTY(int heightHint READ heightHint NOTIFY heightHintChanged)

public:
    explicit QmlInputWidget(QWidget *parent = 0);

  //static void setEmbeddedWidget(QWidget *widget);

  int heightHint() const;

public slots:

signals:
  void heightHintChanged();

private:
  class MultiLineEdit *_input;
  class InputPresenter *_presenter;
};

#endif // QMLINPUTWIDGET_H
