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
  _lastViewportSize = viewportSize();

#if defined(Q_WS_MAEMO_5)
  qDebug() << "scroll setings overshoot/fastVel/decel" << fastVelocityFactor() << decelerationFactor();
  setOvershootPolicy(QAbstractKineticScroller::OvershootAlwaysOff);
  setFastVelocityFactor(1.0);
  setDecelerationFactor(0.1);
#endif


  // TODO get rid of timer when QML scrollbars work
    _timer.setSingleShot(true);
    connect(&_timer,SIGNAL(timeout()), this, SLOT(hideScrollBars()));

    if(_scroll->horizontalScrollBar()) {
      connect(_scroll->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SIGNAL(maximumScrollPositionChanged()));
      connect(_scroll->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(scrollPositionChanged()));
    }
    if(_scroll->verticalScrollBar()) {
      connect(_scroll->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SIGNAL(maximumScrollPositionChanged()));
      connect(_scroll->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(scrollPositionChanged()));
    }

    connect(this, SIGNAL(maximumScrollPositionChanged()), this, SLOT(checkViewportSizeChanged()));
    connect(this, SIGNAL(scrollPositionChanged()), this, SLOT(checkViewportSizeChanged()));
}

void ScrollAreaKineticScroller::checkViewportSizeChanged()
{
  if(viewportSize() != _lastViewportSize) {
    _lastViewportSize = viewportSize();
    emit viewportSizeChanged();
  }
}

QPoint ScrollAreaKineticScroller::maximumScrollPosition() const
{
    return QPoint(_scroll->horizontalScrollBar() ? (_scroll->horizontalScrollBar()->maximum()-_scroll->horizontalScrollBar()->minimum()+_scroll->horizontalScrollBar()->pageStep()) : 0,
                  _scroll->verticalScrollBar() ? (_scroll->verticalScrollBar()->maximum()-_scroll->verticalScrollBar()->minimum()+_scroll->verticalScrollBar()->pageStep()) : 0 );
}

QPoint ScrollAreaKineticScroller::scrollPosition() const
{
  return QPoint(_scroll->horizontalScrollBar() ? maximumScrollPosition().x() - (_scroll->horizontalScrollBar()->maximum() - _scroll->horizontalScrollBar()->value()) : 0,
                _scroll->verticalScrollBar() ? maximumScrollPosition().y() - (_scroll->verticalScrollBar()->maximum() - _scroll->verticalScrollBar()->value()) : 0 );
}

void ScrollAreaKineticScroller::setScrollPosition( const QPoint & pos, const QPoint & overshoot )
{
    Q_UNUSED(overshoot)

  if(_scroll->horizontalScrollBar())
      _scroll->horizontalScrollBar()->setValue(pos.x() - maximumScrollPosition().x() + _scroll->horizontalScrollBar()->maximum());
  if(_scroll->verticalScrollBar())
      _scroll->verticalScrollBar()->setValue(pos.y() - maximumScrollPosition().y() + _scroll->verticalScrollBar()->maximum() + _scroll->verticalScrollBar()->pageStep());

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

int ScrollAreaKineticScroller::scrollbarPos() const
{
  if(maximumScrollPosition().y() <= 0)
    return 0;

  return (viewportSize().height()-scrollbarHeight()) * scrollPosition().y()/(qreal)maximumScrollPosition().y();
}

int ScrollAreaKineticScroller::scrollbarHeight() const
{
  if(maximumScrollPosition().y() <= 0)
    return 10;

  int vertHeight = viewportSize().height() * viewportSize().height()/(qreal)maximumScrollPosition().y();
  if(vertHeight < 10)
    vertHeight = 10;
  return vertHeight;
}

void ScrollAreaKineticScroller::hideScrollBars()
{
    _showBars = false;
    _scroll->viewport()->update();
}

#if defined(Q_WS_MAEMO_5)
void ScrollAreaKineticScroller::stateChanged(QAbstractKineticScroller::State oldState)
{
  bool mvOld = (oldState == QAbstractKineticScroller::Pushing || oldState == QAbstractKineticScroller::AutoScrolling);
  bool mvNew = moving();
  if(mvOld != mvNew) {
    emit movingChanged();
  }
}
#endif

bool ScrollAreaKineticScroller::moving() const
{
#if defined(Q_WS_MAEMO_5)
  return state() == QAbstractKineticScroller::Pushing || state() == QAbstractKineticScroller::AutoScrolling;
#else
  return false;
#endif
}
