#ifndef ITEMVIEWKINETICTAPSUPPRESSOR_H
#define ITEMVIEWKINETICTAPSUPPRESSOR_H

#include <QObject>
#include <QAbstractItemView>

class ScrollAreaKineticScroller;

class ItemViewKineticTapSuppressor : public QObject
{
    Q_OBJECT
public:
    explicit ItemViewKineticTapSuppressor(QAbstractItemView *parent = 0);

    bool handleMouseEvent(QEvent *);

    ScrollAreaKineticScroller *scroller() const { return _scroller; }

signals:

private:
    QPoint _clickStart;
    QAbstractItemView *_view;
    ScrollAreaKineticScroller *_scroller;
};

#endif // ITEMVIEWKINETICTAPSUPPRESSOR_H
