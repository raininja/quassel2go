#include <QDebug>

#include <QMouseEvent>

#if defined(Q_WS_MAEMO_5)
#include <QAbstractKineticScroller>
#endif

#include "scrollareakineticscroller.h"
#include "itemviewkinetictapsuppressor.h"

ItemViewKineticTapSuppressor::ItemViewKineticTapSuppressor(QAbstractItemView *parent) :
    QObject(parent),
    _view(parent),
    _scroller(new ScrollAreaKineticScroller(parent))
{
    // disable the internal itemview scroller...
#if defined(Q_WS_MAEMO_5)
    _view->property("kineticScroller").value<QAbstractKineticScroller *>()->setEnabled(false);
#endif
}

bool ItemViewKineticTapSuppressor::handleMouseEvent(QEvent *event)
{
#if defined(Q_WS_MAEMO_5)
    _view->property("kineticScroller").value<QAbstractKineticScroller *>()->setEnabled(false);

    if(!event)
        return false;

    int oldState = _scroller->state();

    switch(event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
            _scroller->handleMouseEvent((QMouseEvent*)event);
            break;
    default:
            break;
    }

    switch(event->type()) {
    case QEvent::MouseButtonPress:
        if(oldState == QAbstractKineticScroller::AutoScrolling) {
            _clickStart = QPoint();
            return true;
            break;
        }
        _clickStart = ((QMouseEvent*)event)->pos();
        return true;
    case QEvent::MouseButtonRelease: {
        QPoint newPos = ((QMouseEvent*)(event))->pos();
        if(!_clickStart.isNull()) {
            if(_scroller->state() == QAbstractKineticScroller::AutoScrolling) {
                break;
            }
            QModelIndex index = _view->indexAt(newPos);
            _view->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
            _clickStart = QPoint();
            return true;
        }
        break;
    }
    case QEvent::MouseMove: {
        QPoint newPos = ((QMouseEvent*)(event))->pos();
        if(!_clickStart.isNull() && (_scroller->state() == QAbstractKineticScroller::Pushing || _scroller->state() == QAbstractKineticScroller::AutoScrolling) ) {
            _clickStart = QPoint();
            return true;
        } else {
            return true;
        }
        break;
    }
    default:
        ;
    }

#else
    Q_UNUSED(event)
#endif

    return false;
}


