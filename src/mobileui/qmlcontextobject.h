#ifndef QMLCONTEXTOBJECT_H
#define QMLCONTEXTOBJECT_H

#include <QObject>

#include "bufferwidget.h"
//class AbstractBufferContainer;

class QmlContextObject : public QObject
{
    Q_OBJECT

  Q_PROPERTY(int width READ width NOTIFY widthChanged)
  Q_PROPERTY(int height READ height NOTIFY heightChanged)
  Q_PROPERTY(BufferWidget *bufferContainer READ bufferContainer NOTIFY bufferContainerChanged)

public:
    explicit QmlContextObject(QWidget *parent = 0);

  int width() const { return _width; };
  int height() const { return _height; };
  BufferWidget* bufferContainer() const { return _bufferContainer; };

signals:
  void widthChanged(int widht);
  void heightChanged(int height);
  void bufferContainerChanged();

public slots:
  void setBufferContainer(BufferWidget *container);
  void setWidth(int width);
  void setHeight(int height);

protected:
  bool eventFilter(QObject *obj, QEvent *event);

private:
  int _width;
  int _height;
  BufferWidget *_bufferContainer;
};

#endif // QMLCONTEXTOBJECT_H
