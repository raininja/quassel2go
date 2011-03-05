#ifndef QMLINPUTWIDGET_H
#define QMLINPUTWIDGET_H

#include <QGraphicsProxyWidget>
#include <QWidget>

class QmlInputWidget: public QGraphicsProxyWidget
{
    Q_OBJECT

  Q_PROPERTY(int heightHint READ heightHint NOTIFY heightHintChanged)
  Q_PROPERTY(bool focus READ hasFocus WRITE setWidgetFocus NOTIFY widgetFocusChanged)

public:
    explicit QmlInputWidget(QWidget *parent = 0);

  //static void setEmbeddedWidget(QWidget *widget);

  int heightHint() const;

public slots:
  void completeNick();
  void setWidgetFocus(bool focus);

signals:
  void heightHintChanged();
  void widgetFocusChanged();

protected:
  virtual void focusInEvent ( QFocusEvent * event );
  virtual bool focusNextPrevChild ( bool next );
  virtual void focusOutEvent ( QFocusEvent * event );

private:
  class MultiLineEdit *_input;
  class InputPresenter *_presenter;
};

#endif // QMLINPUTWIDGET_H
