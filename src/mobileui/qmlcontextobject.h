#ifndef QMLCONTEXTOBJECT_H
#define QMLCONTEXTOBJECT_H

#include <QObject>

class QmlContextObject : public QObject
{
    Q_OBJECT

  Q_PROPERTY(int width READ width NOTIFY widthChanged)
  Q_PROPERTY(int height READ height NOTIFY heightChanged)

public:
    explicit QmlContextObject(QWidget *parent = 0);

  int width() const { return _width; };
  int height() const { return _height; };

signals:
  void widthChanged(int widht);
  void heightChanged(int height);

public slots:
  void setWidth(int width);
  void setHeight(int height);

protected:
  bool eventFilter(QObject *obj, QEvent *event);

private:
  int _width;
  int _height;
};

#endif // QMLCONTEXTOBJECT_H
