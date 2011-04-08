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
  _activeBufferListModel(new QmlSectionProxyModel(this)),
//  _channelUsersModel(new QmlSectionProxyModel(this)),
  _firstColumn(0),
  _secondColumn(0),
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

void QmlContextObject::setActiveBufferListModel(QAbstractItemModel *model)
{
  _activeBufferListModel->setSourceModel(model);
  emit activeBufferListModelChanged();
}

void QmlContextObject::setActiveBufferListSelectionModel(QItemSelectionModel *model)
{
  if(_activeBufferListSelectionModel) {
    disconnect(_activeBufferListSelectionModel, 0, this, 0);
  }
  _activeBufferListSelectionModel = model;
  if(_activeBufferListSelectionModel) {
    connect(_activeBufferListSelectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SIGNAL(activeBufferListIndexChanged()));
  }
  emit activeBufferListIndexChanged();
}

//void QmlContextObject::setChannelUsersModel(QAbstractItemModel *model)
//{
//  if(_channelUsersModel->sourceModel() == model)
//    return;

//  _channelUsersModel->setSourceModel(model);
//  emit channelUsersModelChanged();
//}

//void QmlContextObject::setChannelUsersRootIndex(const QModelIndex &index)
//{
//  if(_channelUsersRootIndex == index)
//    return;

//  _channelUsersModel->setSourceRootIndex(index);

//  _channelUsersRootIndex= index;
//  emit channelUsersRootIndexChanged();
//}

int QmlContextObject::activeBufferListIndex() const
{
  if(_activeBufferListModel && _activeBufferListSelectionModel)
  {
    return _activeBufferListModel->mapFromSource(_activeBufferListSelectionModel->currentIndex()).row();
  }

  return -1;
}

void QmlContextObject::setActiveBufferListIndex(int index)
{
  if(_activeBufferListModel && _activeBufferListSelectionModel)
  {
    _activeBufferListSelectionModel->setCurrentIndex(_activeBufferListModel->mapToSource(_activeBufferListModel->index(index, 0)),
                                                     QItemSelectionModel::Clear);
  }
}

void QmlContextObject::setCurrentBufferModelIndex(const QModelIndex &index)
{
  setActiveBufferListIndex(_activeBufferListModel->mapFromSource(index).row());
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
