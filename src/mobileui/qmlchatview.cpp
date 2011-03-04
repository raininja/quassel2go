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
  _scrollModel(new QmlScrollModel(this))
{
  Q_UNUSED(parent)

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
    // connect ...
  }
}

void QmlChatView::requestBacklog()
{
  if(!_currentView)
    return;

  _currentView->scene()->requestBacklog();
}
