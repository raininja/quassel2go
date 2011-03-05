#ifndef QMLNICKWIDGET_H
#define QMLNICKWIDGET_H

#include <QGraphicsProxyWidget>

class QmlNickWidget : public QGraphicsProxyWidget
{
    Q_OBJECT

  Q_PROPERTY(bool focus READ hasFocus WRITE setWidgetFocus NOTIFY widgetFocusChanged)
public:
    explicit QmlNickWidget(QWidget *parent = 0);

public slots:
  void setWidgetFocus(bool focus);

signals:
  void widgetFocusChanged();

protected:
  virtual void focusInEvent ( QFocusEvent * event );
  virtual bool focusNextPrevChild ( bool next );
  virtual void focusOutEvent ( QFocusEvent * event );

private:
  class QComboBox *_nick;
  class InputNickPresenter *_presenter;
};

#endif // QMLNICKWIDGET_H
