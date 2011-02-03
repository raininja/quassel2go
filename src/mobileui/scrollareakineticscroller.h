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

  Q_PROPERTY(QPoint scrollPosition READ scrollPosition NOTIFY scrollPositionChanged)
  Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
  Q_PROPERTY(QSize viewportSize READ viewportSize NOTIFY viewportSizeChanged)
  Q_PROPERTY(QPoint maximumScrollPosition READ maximumScrollPosition NOTIFY maximumScrollPositionChanged)
  Q_PROPERTY(int scrollbarPos READ scrollbarPos NOTIFY viewportSizeChanged)
  Q_PROPERTY(int scrollbarHeight READ scrollbarHeight NOTIFY viewportSizeChanged)
public:
    explicit ScrollAreaKineticScroller(QAbstractScrollArea *parent);

    Q_INVOKABLE QPoint maximumScrollPosition() const;
    QPoint scrollPosition() const;
    Q_INVOKABLE QSize viewportSize() const;

    int scrollbarPos() const;
    int scrollbarHeight() const;

    //bool handleMouseEvent(QEvent *e);

    bool handleMouseEvent ( QMouseEvent * event );

    void paintEvent(QPaintEvent *e);

    bool moving() const;

    void setScrollPosition( const QPoint & pos, const QPoint & overshoot );
protected slots:
    void hideScrollBars();

    void checkViewportSizeChanged();


#if defined(Q_WS_MAEMO_5)
    void stateChanged(QAbstractKineticScroller::State oldState);
#endif

signals:
    void scrollPositionChanged();
    void movingChanged();
    void maximumScrollPositionChanged();
    void viewportSizeChanged();

private:
#if defined(Q_WS_MAEMO_5)
    QAbstractScrollArea *_scroll;
#else
    QAbstractScrollArea *_scroll;
#endif
    bool _showBars;
    QTimer _timer;
    QSize _lastViewportSize;
};

#endif // SCROLLAREAKINETICSCROLLER_H
