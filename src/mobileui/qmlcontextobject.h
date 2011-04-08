#ifndef QMLCONTEXTOBJECT_H
#define QMLCONTEXTOBJECT_H

#include <QObject>
#include <QPointer>
#include <QAbstractProxyModel>

#include <QModelIndex>
#include <QItemSelectionModel>

//#include "bufferwidget.h"
#include "qmlsectionproxymodel.h"

class QmlContextObject : public QObject
{
    Q_OBJECT

  Q_PROPERTY(QAbstractItemModel *allBuffersModel READ allBuffersModel NOTIFY activeBufferListModelChanged)
//  Q_PROPERTY(QAbstractItemModel *channelUsersModel READ channelUsersModel NOTIFY channelUsersModelChanged)
//  Q_PROPERTY(QModelIndex channelUsersRootIndex READ channelUsersRootIndex NOTIFY channelUsersRootIndexChanged)
  Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged)
  Q_PROPERTY(int activeBufferListIndex READ activeBufferListIndex WRITE setActiveBufferListIndex NOTIFY activeBufferListIndexChanged)

  Q_PROPERTY(bool searchBarVisible READ searchBarVisible NOTIFY searchBarVisibleChanged)

  Q_PROPERTY(QObject *firstColumn READ firstColumn NOTIFY firstColumnChanged)
  Q_PROPERTY(QObject *secondColumn READ secondColumn NOTIFY secondColumnChanged)
public:
    explicit QmlContextObject(QObject *parent = 0);

  void setBufferWidget(class BufferWidget* widget);

  bool fullScreen() const { return _fullScreen; }
  QAbstractItemModel* allBuffersModel() const { return _activeBufferListModel; }
//  QAbstractItemModel* channelUsersModel() const { return _channelUsersModel; }
  QModelIndex channelUsersRootIndex() const { return _channelUsersRootIndex; }
  int activeBufferListIndex() const;

  QObject *firstColumn() const { return _firstColumn; }
  QObject *secondColumn() const { return _secondColumn; }

  bool searchBarVisible() const;

signals:
  void fullScreenChanged(bool fullScreen);
  void activeBufferListModelChanged();
  void channelUsersModelChanged();
  void channelUsersRootIndexChanged();
  void activeBufferListIndexChanged();

  void requestZoomIn();
  void requestZoomOut();
  void requestSearch();

  void firstColumnChanged();
  void secondColumnChanged();

  void searchBarVisibleChanged();

public slots:
  void setFullScreen(bool fullScreen);
  void setActiveBufferListModel(QAbstractItemModel *model);
  void setActiveBufferListSelectionModel(QItemSelectionModel *model);
//  void setChannelUsersModel(QAbstractItemModel *model);
//  void setChannelUsersRootIndex(const QModelIndex &index);
  void setActiveBufferListIndex(int index);
  void setCurrentBufferModelIndex(const QModelIndex &index);

  void setFirstColumn(QObject *obj);
  void setSecondColumn(QObject *obj);

  void zoomIn();
  void zoomOut();
  void search();

protected:

private:
  bool _fullScreen;
  QPointer<QAbstractProxyModel> _activeBufferListModel;
  QPointer<QItemSelectionModel> _activeBufferListSelectionModel;
//  QPointer<QmlSectionProxyModel> _channelUsersModel;
  QPointer<QObject> _firstColumn;
  QPointer<QObject> _secondColumn;
  QModelIndex _channelUsersRootIndex;

  QPointer<BufferWidget> _bufferWidget;
};

#endif // QMLCONTEXTOBJECT_H
