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

  Q_PROPERTY(QAbstractItemModel *allBuffersModel READ allBuffersModel NOTIFY allBuffersModelChanged)
  Q_PROPERTY(QAbstractItemModel *channelUsersModel READ channelUsersModel NOTIFY channelUsersModelChanged)
  Q_PROPERTY(QModelIndex channelUsersRootIndex READ channelUsersRootIndex NOTIFY channelUsersRootIndexChanged)
  Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged)
  Q_PROPERTY(int currentBufferIndex READ currentBufferIndex WRITE setCurrentBufferIndex NOTIFY currentBufferIndexChanged)
public:
    explicit QmlContextObject(QObject *parent = 0);

  bool fullScreen() const { return _fullScreen; }
  QAbstractItemModel* allBuffersModel() const { return _allBuffersModel; }
  QAbstractItemModel* channelUsersModel() const { return _channelUsersModel; }
  QModelIndex channelUsersRootIndex() const { return _channelUsersRootIndex; }
  int currentBufferIndex() const;

signals:
  void fullScreenChanged(bool fullScreen);
  void allBuffersModelChanged();
  void channelUsersModelChanged();
  void channelUsersRootIndexChanged();
  void currentBufferIndexChanged();
  void currentBufferModelIndexChanged(const QModelIndex &index, QItemSelectionModel::SelectionFlags flags);

public slots:
  void setFullScreen(bool fullScreen);
  void setAllBuffersModel(QAbstractItemModel *model);
  void setChannelUsersModel(QAbstractItemModel *model);
  void setChannelUsersRootIndex(const QModelIndex &index);
  void setCurrentBufferIndex(int index);
  void setCurrentBufferModelIndex(const QModelIndex &index);

protected:

private:
  bool _fullScreen;
  QPointer<QAbstractProxyModel> _allBuffersModel;
  QPointer<QmlSectionProxyModel> _channelUsersModel;
  QModelIndex _channelUsersRootIndex;
  int _currentBufferIndex;
};

#endif // QMLCONTEXTOBJECT_H
