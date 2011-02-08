#include <QAbstractItemModel>
#include <QModelIndex>
#include <QDebug>

#include "qmlsectionproxymodel.h"

QmlSectionProxyModel::QmlSectionProxyModel(QObject *parent) :
    QAbstractProxyModel(parent)
{
}

QModelIndex QmlSectionProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if(!sourceModel())
        return QModelIndex();

    // always map to the only column 0

    const int rootCnt = sourceModel()->rowCount(_root);
    int cnt = 0;
    bool success = false;
    for(int i = 0; i < rootCnt; ++i) {
      QModelIndex sectionIndex = sourceModel()->index(i, 0, _root);

      if(sectionIndex == sourceIndex) {
        cnt += 0;
        success = true;
        break;
      } else if(sectionIndex == sourceIndex.parent()) {
        cnt += 1+ sourceIndex.row();
        success = true;
        break;
      } else {
        cnt += 1+ sourceModel()->rowCount(sectionIndex);
      }
    }

    if(success)
      return index(cnt, 0, _root);
    else
      return QModelIndex();
}

QModelIndex QmlSectionProxyModel::_mapToSource(int row) const {
  if(!sourceModel())
      return QModelIndex();

  // always map to first column

  const int rootCnt = sourceModel()->rowCount(_root);
  int cnt = 0;
  for(int i = 0; i < rootCnt; ++i) {
    QModelIndex sectionIndex = sourceModel()->index(i, 0, _root);
    int sectionCount = sourceModel()->rowCount(sectionIndex);

    if(row == cnt) {
      return sectionIndex;
    } else if(row >= 1+cnt && row <= (1+cnt+sectionCount-1)) {
      return sourceModel()->index(row-cnt-1, 0, sectionIndex);
    }

    cnt += 1 + sectionCount;
  }

  return QModelIndex();
}

QModelIndex QmlSectionProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
  // always map to first column
  return _mapToSource(proxyIndex.row());
}

int QmlSectionProxyModel::columnCount(const QModelIndex &parent) const
{
    if(!sourceModel())
        return 0;
    if(parent.isValid())
        return 0;
    return 1;
}

int QmlSectionProxyModel::rowCount(const QModelIndex &parent) const
{
    if(!sourceModel())
        return 0;
    if(parent.isValid())
        return 0;

    const int rootCnt = sourceModel()->rowCount(_root);
    int cnt = rootCnt;
    for(int i = 0; i < rootCnt; ++i) {
      cnt += sourceModel()->rowCount(sourceModel()->index(i, 0, _root));
    }

    return cnt;
}

void QmlSectionProxyModel::setSourceModel ( QAbstractItemModel * sourceModel )
{
  beginResetModel();

  if(this->sourceModel()) {
    disconnect(this->sourceModel());
  }

  QAbstractProxyModel::setSourceModel(sourceModel);
  if(sourceModel) {
    QHash<int, QByteArray> roles = roleNames(); // = sourceModel->roleNames();
    roles[Qt::UserRole+999] = "section_item";
    roles[Qt::UserRole+1000] = "section_depth";
    setRoleNames(roles);

    connect(sourceModel, SIGNAL(modelReset()), this, SLOT(_mdlReset()));

    // this is likely to be rather inefficient for larger source models
    connect(sourceModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(_mdlReset()));
    connect(sourceModel, SIGNAL(rowsMoved(const QModelIndex &,int,int, const QModelIndex &, int)), this, SLOT(_mdlReset()));
    connect(sourceModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(_mdlReset()));
    connect(sourceModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(_mdlReset()));
  }
  _root = QModelIndex();

  reset();

  endResetModel();
}

void QmlSectionProxyModel::_mdlReset()
{
  reset();
}

QVariant QmlSectionProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if(!sourceModel())
        return QVariant();

    // section item mapping
    switch(role) {
    case Qt::UserRole+999: {
      QModelIndex srcIndex = mapToSource(proxyIndex);
      if(srcIndex.parent() == QModelIndex())
        return QVariant();
      else
        return sourceModel()->data(srcIndex.parent(),
                                   Qt::DisplayRole);
    }
    case Qt::UserRole+1000: {
      QModelIndex srcIndex = mapToSource(proxyIndex);
      if(srcIndex.parent() == _root) {
        return 0;
      } else {
        return 1;
      }
    }
    default:
      break;
    }
    return sourceModel()->data(mapToSource(proxyIndex), role);
}

QModelIndex QmlSectionProxyModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QModelIndex QmlSectionProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!sourceModel() || parent.isValid())
        return QModelIndex();
    return createIndex(row, column, _mapToSource(row).internalPointer());
}

void QmlSectionProxyModel::setSourceRootIndex(const QModelIndex &root)
{
  if(_root == root)
    return;

  beginResetModel();

  _root = root;

  endResetModel();
}
