#ifndef SCROLLAREAKINETICSCROLLER_H
#define SCROLLAREAKINETICSCROLLER_H

#include <QAbstractScrollArea>

#if defined(Q_WS_MAEMO_5)
#include <QAbstractKineticScroller>
#endif

#include <QTimer>

class ScrollAreaKineticScroller : public QObject
    #if defined(Q_WS_MAEMO_5)
    , public QAbstractKineticScroller
    #endif
{
    Q_OBJECT
public:
    explicit ScrollAreaKineticScroller(QAbstractScrollArea *parent);

    QPoint maximumScrollPosition() const;
    QPoint scrollPosition() const;
    void setScrollPosition( const QPoint & pos, const QPoint & overshoot );
    QSize viewportSize() const;

    //bool handleMouseEvent(QEvent *e);

    bool handleMouseEvent ( QMouseEvent * event );

    void paintEvent(QPaintEvent *e);

protected slots:
    void hideScrollBars();

signals:

private:
#if defined(Q_WS_MAEMO_5)
    QAbstractScrollArea *_scroll;
#else
    QAbstractScrollArea *_scroll;
#endif
    bool _showBars;
    QTimer _timer;
};

#endif // SCROLLAREAKINETICSCROLLER_H
