#include <QEvent>
#include <QResizeEvent>
#include <QWidget>
#include <QDebug>

#include "qmlsectionproxymodel.h"

#include "qmlcontextobject.h"

QmlContextObject::QmlContextObject(QWidget *parent) :
    QObject(parent),
  _bufferContainer(0),
  _fullScreen(false),
  _allBuffersModel(new QmlSectionProxyModel(this)),
  _channelUsersModel(new QmlSectionProxyModel(this))
{
}

void QmlContextObject::setBufferContainer(BufferWidget *container)
{
  if(_bufferContainer != container) {
    _bufferContainer = container;
    emit bufferContainerChanged();
  }
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
  if(_allBuffersModel->sourceModel() == model)
    return;

  _allBuffersModel->setSourceModel(model);
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
