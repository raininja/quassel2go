#include <QEvent>
#include <QResizeEvent>
#include <QWidget>
#include <QDebug>

#include "qmlsectionproxymodel.h"

#include "qmlcontextobject.h"

QmlContextObject::QmlContextObject(QObject *parent) :
    QObject(parent),
  //_bufferContainer(0),
  _fullScreen(false),
  _allBuffersModel(new QmlSectionProxyModel(this)),
  _channelUsersModel(new QmlSectionProxyModel(this)),
  _currentBufferIndex(-1)
{
}

void QmlContextObject::setFullScreen(bool fullScreen)
{
  if(_fullScreen == fullScreen)
    return;

  _fullScreen = fullScreen;
  emit fullScreenChanged(fullScreen);
}

void QmlContextObject::setAllBuffersModel(QAbstractItemModel *model)
{
//  if(_allBuffersModel->sourceModel() == model)
//    return;

  _allBuffersModel->setSourceModel(model);
  emit allBuffersModelChanged();
}

void QmlContextObject::setChannelUsersModel(QAbstractItemModel *model)
{
  if(_channelUsersModel->sourceModel() == model)
    return;

  _channelUsersModel->setSourceModel(model);
  emit channelUsersModelChanged();
}

void QmlContextObject::setChannelUsersRootIndex(const QModelIndex &index)
{
  if(_channelUsersRootIndex == index)
    return;

  _channelUsersModel->setSourceRootIndex(index);

  _channelUsersRootIndex= index;
  emit channelUsersRootIndexChanged();
}

int QmlContextObject::currentBufferIndex() const
{
  return _currentBufferIndex;
}

void QmlContextObject::setCurrentBufferIndex(int index)
{
  if(_currentBufferIndex == index)
    return;

  _currentBufferIndex = index;

  emit currentBufferModelIndexChanged(_allBuffersModel->mapToSource(_allBuffersModel->index(index, 0)),
                                      QItemSelectionModel::Clear);
  emit currentBufferIndexChanged();
}

void QmlContextObject::setCurrentBufferModelIndex(const QModelIndex &index)
{
  setCurrentBufferIndex(_allBuffersModel->mapFromSource(index).row());
}
