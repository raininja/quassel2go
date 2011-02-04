#include "qmlchatview.h"
#include "chatscene.h"
#include "chatview.h"
#include <QDebug>
#include "qmlscrollmodel.h"

#include <QListWidget>

#include "scrollareakineticscroller.h"

QWidget *QmlChatView::_bufferWidget = 0;

QmlChatView::QmlChatView(QWidget *parent) :
    QGraphicsProxyWidget(),
  _currentView(0),
  _scrollModel(new QmlScrollModel(this)) /*,
  _id(-1),
  _par(parent)*/
{
  if(_bufferWidget) {
    setWidget(_bufferWidget);
    if(qobject_cast<QAbstractScrollArea*>(_bufferWidget)) {
      _scrollModel->setScrollArea(qobject_cast<QAbstractScrollArea*>(_bufferWidget));
    }
    connect(_bufferWidget, SIGNAL(currentChatViewChanged(ChatView*)), this, SLOT(currentChatViewChanged(ChatView*)));
  } else {
    qWarning() << "QmlChatView needs to be initialized first with ::setBufferWidget()";
    setWidget(new QWidget());
  }
}

void QmlChatView::setBufferWidget(QWidget *widget)
{
  _bufferWidget = widget;
}

QmlScrollModel *QmlChatView::scrollModel() const
{
  return _scrollModel;
}

void QmlChatView::currentChatViewChanged(ChatView *view)
{
  if(!_bufferWidget)
    return;

  if(_currentView == view)
    return;

  if(_currentView) {
    disconnect(_currentView);
  }
  _currentView = view;
  _scrollModel->setScrollArea(view);

  if(view) {
    connect(view->scroller(), SIGNAL(scrollPositionChanged()), this, SIGNAL(scrollChanged()));
    connect(view->scroller(), SIGNAL(movingChanged()), this, SIGNAL(scrollMovingChanged()));
  }
}

int QmlChatView::scrollbarPos() const
{
  if(!_bufferWidget || !_currentView)
    return 0;

  if(_currentView->scroller()->maximumScrollPosition().y() <= 0)
    return 0;

  return _currentView->scroller()->scrollbarPos();
}

int QmlChatView::scrollbarHeight() const
{
  if(!_bufferWidget || !_currentView)
    return 15;

  return _currentView->scroller()->scrollbarHeight();
}

bool QmlChatView::scrollbarMoving() const
{
  if(!_bufferWidget || !_currentView)
    return false;

  return _currentView->scroller()->moving();
}

int QmlChatView::contentsHeight() const
{
  if(!_bufferWidget || !_currentView || !_currentView->scroller())
    return 0;

  return _currentView->scroller()->maximumScrollPosition().y() + 10;
}

void QmlChatView::setPos(qreal pos)
{
  if(!_bufferWidget || !_currentView || !_currentView->scroller())
    return;

  QPoint point = QPoint(0,pos);
//  QPoint over = QPoint(0,0);
//  if(pos < 0) {
//    point = QPoint(0,0);
//    over = QPoint(0,pos);
//  } else if (pos > contentsHeight()) {
//    point = QPoint(0,contentsHeight());
//    over = QPoint(0,pos-contentsHeight());
//  }
  _currentView->scroller()->setScrollPosition(point, QPoint());
}

void QmlChatView::requestBacklog()
{
  if(!_currentView)
    return;

  _currentView->scene()->requestBacklog();
}
