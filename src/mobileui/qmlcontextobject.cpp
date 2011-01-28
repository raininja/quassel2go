#include <QEvent>
#include <QResizeEvent>
#include <QWidget>
#include <QDebug>

#include "qmlcontextobject.h"

QmlContextObject::QmlContextObject(QWidget *parent) :
    QObject(parent),
  _width(0),
  _height(0)
{
  parent->installEventFilter(this);
}

void QmlContextObject::setWidth(int width) {
  if(width != _width) {
    _width = width;
    emit widthChanged(width);
  }
}

void QmlContextObject::setHeight(int height) {
  if(height != _height) {
    _height = height;
    emit heightChanged(height);
  }
}

bool QmlContextObject::eventFilter(QObject *obj, QEvent *event) {
  // track widget geometry changes


  QWidget *w = 0;
  if(obj == parent() && (w = qobject_cast<QWidget*>(obj))) {


    switch(event->type()) {
    case QEvent::Resize: {
      QResizeEvent *re = (QResizeEvent*)event;
      setWidth(re->size().width());
      setHeight(re->size().height());
      break;
    }
    default:
      break;
    }
  }

  return false;
}
