#ifndef SCROLLAREAKINETICSCROLLER_H
#define SCROLLAREAKINETICSCROLLER_H

#include <QAbstractScrollArea>
#include <QAbstractKineticScroller>
#include <QTimer>

class ScrollAreaKineticScroller : public QObject, public QAbstractKineticScroller
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
    QAbstractScrollArea *_scroll;
    bool _showBars;
    QTimer _timer;
};

#endif // SCROLLAREAKINETICSCROLLER_H
