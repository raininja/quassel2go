#include "qmlchatview.h"

QWidget *QmlChatView::_bufferWidget = 0;

QmlChatView::QmlChatView(QWidget *parent) :
    QGraphicsProxyWidget()/*,
  _id(-1),
  _par(parent)*/
{
  setWidget(_bufferWidget);
}

//void QmlChatView::setBufferId(BufferId id)
//{
//  if(widget())
//    widget()->deleteLater();
//  setWidget(new ChatView(_id, _par));
//}

void QmlChatView::setBufferWidget(QWidget *widget)
{
  _bufferWidget = widget;
}
