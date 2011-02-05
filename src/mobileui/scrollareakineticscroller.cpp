#include <QMouseEvent>
#include <QScrollBar>
#include <QPainter>
#include <QDebug>

#include "scrollareakineticscroller.h"

ScrollAreaKineticScroller::ScrollAreaKineticScroller(QAbstractScrollArea *parent) :
    QObject(parent),
    _scroll(parent),
    _showBars(false)
{
    _timer.setSingleShot(true);
    connect(&_timer,SIGNAL(timeout()), this, SLOT(hideScrollBars()));
}

QPoint ScrollAreaKineticScroller::maximumScrollPosition() const
{
    return QPoint(_scroll->horizontalScrollBar() ? _scroll->horizontalScrollBar()->maximum() : 0,
                  _scroll->verticalScrollBar() ? _scroll->verticalScrollBar()->maximum() : 0 );
}

QPoint ScrollAreaKineticScroller::scrollPosition() const
{
  return QPoint(_scroll->horizontalScrollBar() ? _scroll->horizontalScrollBar()->value() : 0,
                _scroll->verticalScrollBar() ? _scroll->verticalScrollBar()->value() : 0 );
}

void ScrollAreaKineticScroller::setScrollPosition( const QPoint & pos, const QPoint & overshoot )
{
    Q_UNUSED(overshoot)

  if(_scroll->horizontalScrollBar())
      _scroll->horizontalScrollBar()->setValue(pos.x());
  if(_scroll->verticalScrollBar())
      _scroll->verticalScrollBar()->setValue(pos.y());

  _showBars = true;
  _scroll->viewport()->update();
  _timer.start(500);
}

QSize ScrollAreaKineticScroller::viewportSize() const
{
  return _scroll->viewport()->size();
}

bool ScrollAreaKineticScroller::handleMouseEvent ( QMouseEvent * event )
{
#if defined(Q_WS_MAEMO_5)
    return QAbstractKineticScroller::handleMouseEvent(event);
#else
    return false;
#endif
}

void ScrollAreaKineticScroller::paintEvent(QPaintEvent *e)
{
    if(_showBars && maximumScrollPosition().y() > 0) {
        QPainter p(_scroll->viewport());
        int barThickness = 5;
        int vertHeight = viewportSize().height() * viewportSize().height()/maximumScrollPosition().y();
        if(vertHeight < 4)
          vertHeight = 4;
        int barPos = (viewportSize().height()-vertHeight) * scrollPosition().y()/maximumScrollPosition().y();
        QRect r(viewportSize().width()-barThickness, barPos, barThickness, vertHeight);
        p.fillRect(r, Qt::gray);
    }
}

void ScrollAreaKineticScroller::hideScrollBars()
{
    _showBars = false;
    _scroll->viewport()->update();
}
