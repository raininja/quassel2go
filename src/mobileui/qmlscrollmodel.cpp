#include "qmlscrollmodel.h"

#include <QMouseEvent>
#include <QResizeEvent>

#include <QScrollBar>
#include <QDebug>

QmlScrollModel::QmlScrollModel(QObject *parent) :
    QObject(parent),
    _scrollArea(0)
{
}

void QmlScrollModel::setScrollArea(QAbstractScrollArea *scrollArea)
{
  if(scrollArea == _scrollArea)
    return;

  // disconnect
  if(_scrollArea) {
    disconnect(_scrollArea->horizontalScrollBar(), 0, this, 0);
    disconnect(_scrollArea->verticalScrollBar(), 0, this, 0);
    _scrollArea->removeEventFilter(this);
  }

  // connect
  _scrollArea = scrollArea;
  if(_scrollArea) {
    if(_scrollArea->horizontalScrollBar()) {
      connect(_scrollArea->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SIGNAL(contentsSizeChanged()));
      connect(_scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(horizontalPositionChanged()));
    }
    if(_scrollArea->verticalScrollBar()) {
      connect(_scrollArea->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SIGNAL(contentsSizeChanged()));
      connect(_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(verticalPositionChanged()));
    }

    _scrollArea->viewport()->installEventFilter(this);
  }

  // notify
  emit contentsSizeChanged();
  emit viewportSizeChanged();
  emit horizontalPositionChanged();
  emit verticalPositionChanged();
}

int QmlScrollModel::scrollRangeX() const
{
  if(!_scrollArea || !_scrollArea->horizontalScrollBar())
    return 0;
  return _scrollArea->horizontalScrollBar()->maximum()-
           _scrollArea->horizontalScrollBar()->minimum();
}

int QmlScrollModel::scrollRangeY() const
{
  if(!_scrollArea || !_scrollArea->verticalScrollBar())
    return 0;
  return _scrollArea->verticalScrollBar()->maximum()-
           _scrollArea->verticalScrollBar()->minimum();
}

int QmlScrollModel::horizontalPosition() const
{
  if(!_scrollArea || !_scrollArea->horizontalScrollBar() ||
      contentsSize().width() <= 0)
    return 0;

     return (_scrollArea->horizontalScrollBar()->value() - _scrollArea->horizontalScrollBar()->minimum());
}

int QmlScrollModel::verticalPosition() const
{
  if(!_scrollArea || !_scrollArea->verticalScrollBar() ||
      contentsSize().height() <= 0)
    return 0;

     return (_scrollArea->verticalScrollBar()->value() - _scrollArea->verticalScrollBar()->minimum());
}

void QmlScrollModel::setHorizontalPosition(int pos)
{
  if(!_scrollArea || !_scrollArea->viewport() ||
      !_scrollArea->horizontalScrollBar() ||
      scrollRangeY() <= 0)
    return;

  int scrollPos = (pos + _scrollArea->horizontalScrollBar()->minimum());
  _scrollArea->horizontalScrollBar()->setValue(scrollPos);
}

void QmlScrollModel::setVerticalPosition(int pos)
{
  if(!_scrollArea || !_scrollArea->viewport() ||
      !_scrollArea->verticalScrollBar() ||
      scrollRangeY() <= 0)
    return;

  int scrollPos = (pos + _scrollArea->verticalScrollBar()->minimum());
  _scrollArea->verticalScrollBar()->setValue(scrollPos);
}

QPoint QmlScrollModel::viewportPosition() const
{
  if(!_scrollArea || !_scrollArea->viewport())
    return QPoint();

  return _scrollArea->viewport()->pos();
}

QSize QmlScrollModel::viewportSize() const
{
  if(!_scrollArea || !_scrollArea->viewport())
    return QSize();

  return _scrollArea->viewport()->size();
}

QSize QmlScrollModel::contentsSize() const
{
  if(!_scrollArea)
    return QSize();

  int h = 0;
  int w = 0;
  if(_scrollArea->horizontalScrollBar()) {
    w = scrollRangeX(); // +_scrollArea->horizontalScrollBar()->pageStep();
  }
  if(_scrollArea->verticalScrollBar()) {
    h = scrollRangeY(); // +_scrollArea->verticalScrollBar()->pageStep();
  }

  return QSize(w, h);
}

bool QmlScrollModel::eventFilter(QObject *object, QEvent *event)
{
  if(!object || !event || !_scrollArea || object != _scrollArea->viewport())
    return false;

  switch(event->type()) {
  case QEvent::Resize:
    emit viewportSizeChanged();
  case QEvent::Move:
    emit viewportPositionChanged();
  default:
    break;
  }

  return false;
}
