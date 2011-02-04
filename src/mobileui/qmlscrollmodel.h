#ifndef QMLSCROLLMODEL_H
#define QMLSCROLLMODEL_H

#include <QObject>
#include <QAbstractScrollArea>

class QAbstractScrollArea;

class QmlScrollModel : public QObject
{
    Q_OBJECT

  Q_PROPERTY(int horizontalPosition READ horizontalPosition WRITE setHorizontalPosition NOTIFY horizontalPositionChanged)
  Q_PROPERTY(int verticalPosition READ verticalPosition WRITE setVerticalPosition NOTIFY verticalPositionChanged)
  Q_PROPERTY(QPoint viewportPosition READ viewportPosition NOTIFY viewportPositionChanged)
  Q_PROPERTY(QSize viewportSize READ viewportSize NOTIFY viewportSizeChanged)
  Q_PROPERTY(QSize contentsSize READ contentsSize NOTIFY contentsSizeChanged)
public:
    explicit QmlScrollModel(QObject *parent = 0);

  void setScrollArea(QAbstractScrollArea *scrollArea);

    int horizontalPosition() const;
    int verticalPosition() const;
    QPoint viewportPosition() const;
    QSize viewportSize() const;
    QSize contentsSize() const;

public slots:
    void setHorizontalPosition(int pos);
    void setVerticalPosition(int pos);

protected:
    virtual bool eventFilter(QObject *object, QEvent *event);

signals:
    void horizontalPositionChanged();
    void verticalPositionChanged();
    void viewportPositionChanged();
    void viewportSizeChanged();
    void contentsSizeChanged();

private:
    int scrollRangeX() const;
    int scrollRangeY() const;
    QAbstractScrollArea *_scrollArea;
};

#endif // QMLSCROLLMODEL_H
