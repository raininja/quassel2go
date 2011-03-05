#include <QEvent>
#include <QResizeEvent>
#include <QWidget>
#include <QDebug>

#include "bufferwidget.h"

#include "qmlsectionproxymodel.h"

#include "qmlcontextobject.h"

QmlContextObject::QmlContextObject(QObject *parent) :
    QObject(parent),
  //_bufferContainer(0),
  _fullScreen(false),
  _allBuffersModel(new QmlSectionProxyModel(this)),
  _channelUsersModel(new QmlSectionProxyModel(this)),
  _firstColumn(0),
  _secondColumn(0),
  _currentBufferIndex(-1),
  _bufferWidget(0)
{
}

void QmlContextObject::setBufferWidget(class BufferWidget* widget)
{
  if(_bufferWidget) {
    disconnect(_bufferWidget);
  }

  _bufferWidget = widget;

  if(_bufferWidget) {
    connect(_bufferWidget, SIGNAL(searchBarVisibleChanged()), this, SIGNAL(searchBarVisibleChanged()));
  }
}

bool QmlContextObject::searchBarVisible() const
{
  if(!_bufferWidget)
    return false;

  return _bufferWidget->searchBarVisible();
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

void QmlContextObject::zoomIn()
{
  emit requestZoomIn();
}

void QmlContextObject::zoomOut()
{
  emit requestZoomOut();
}
void QmlContextObject::search()
{
  emit requestSearch();
}

void QmlContextObject::setFirstColumn(QObject *obj)
{
  _firstColumn = obj;
  emit firstColumnChanged();
}

void QmlContextObject::setSecondColumn(QObject *obj)
{
  _secondColumn = obj;
  emit secondColumnChanged();
}
