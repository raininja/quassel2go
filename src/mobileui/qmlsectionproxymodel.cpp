#include <QAbstractItemModel>
#include <QModelIndex>
#include <QDebug>

#include "qmlsectionproxymodel.h"

QmlSectionProxyModel::QmlSectionProxyModel(QObject *parent) :
    QAbstractProxyModel(parent),
    _rowCount(0)
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

    connect(sourceModel, SIGNAL(rowsAboutToBeInserted (const QModelIndex &, int, int)),
            this, SLOT(srcRowsAboutToBeInserted(QModelIndex,int,int)));
    connect(sourceModel, SIGNAL(rowsAboutToBeRemoved ( const QModelIndex &, int, int )),
            this, SLOT(srcRowsAboutToBeRemoved(const QModelIndex &, int, int)));
    connect(sourceModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
            this, SLOT(srcRowsInserted(QModelIndex,int,int)));
    connect(sourceModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
            this, SLOT(srcRowsRemoved(QModelIndex,int,int)));
    connect(sourceModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(srcDataChanged(QModelIndex,QModelIndex)));

    connect(sourceModel, SIGNAL(layoutChanged()), this, SLOT(srcLayoutChanged()));
    connect(sourceModel, SIGNAL(layoutAboutToBeChanged()), this, SLOT(srcLayoutAboutToBeChanged()));

    // this is likely to be rather inefficient for larger source models
    //    connect(sourceModel, SIGNAL(rowsMoved(const QModelIndex &,int,int, const QModelIndex &, int)), this, SLOT(_mdlReset()));
    // connect(sourceModel, SIGNAL(headerDataChanged(Qt::Orientation, int, int)), this, SLOT(_mdlReset()));
    // connect(sourceModel, SIGNAL(columnsInserted (const QModelIndex &, int, int)), this, SLOT(_mdlReset()));
  }
  _root = QModelIndex();

  reset();
  _rowCount = rowCount(QModelIndex());

  endResetModel();
}

void QmlSectionProxyModel::_mdlReset()
{
  beginResetModel();
  reset();
  _rowCount = rowCount(QModelIndex());
  endResetModel();
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
    return createIndex(row, column);
}

void QmlSectionProxyModel::setSourceRootIndex(const QModelIndex &root)
{
  if(_root == root)
    return;

  beginResetModel();

  _root = root;

  endResetModel();
}

void QmlSectionProxyModel::srcLayoutAboutToBeChanged()
{
  emit layoutAboutToBeChanged();

//  beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()));
//  endRemoveRows();
}

void QmlSectionProxyModel::srcLayoutChanged()
{
  int newCnt = rowCount(QModelIndex());
  if(newCnt > _rowCount) {
    if(_rowCount > 0) {
      emit dataChanged(index(0,0), index(_rowCount-1,0));
    }
    beginInsertRows(QModelIndex(), _rowCount, newCnt-1);
    endInsertRows();
  } else if(newCnt < _rowCount) {
    if(newCnt > 0) {
      emit dataChanged(index(0,0), index(newCnt-1,0));
    }
    beginRemoveRows(QModelIndex(), newCnt, _rowCount-1);
    endRemoveRows();
  } else {
    if(_rowCount > 0) {
      emit dataChanged(index(0,0), index(_rowCount-1,0));
    }
  }
  _rowCount = newCnt;

  emit layoutChanged();
}

void QmlSectionProxyModel::srcRowsAboutToBeInserted ( const QModelIndex & parent, int start, int end )
{
  Q_UNUSED(parent)
  Q_UNUSED(start)
  Q_UNUSED(end)
}

void QmlSectionProxyModel::srcRowsAboutToBeMoved ( const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationColumn )
{
  Q_UNUSED(sourceParent)
  Q_UNUSED(sourceStart)
  Q_UNUSED(sourceEnd)
  Q_UNUSED(destinationParent)
  Q_UNUSED(destinationColumn)
}

void QmlSectionProxyModel::srcRowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{
  Q_UNUSED(parent)
  Q_UNUSED(start)
  Q_UNUSED(end)
}

void QmlSectionProxyModel::srcRowsInserted ( const QModelIndex & parent, int start, int end )
{
  Q_UNUSED(parent)
  Q_UNUSED(start)
  Q_UNUSED(end)

  // TODO: more efficiently calculate the index range and call inserted etc.
_mdlReset();
}

void QmlSectionProxyModel::srcRowsMoved ( const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationColumn )
{
  Q_UNUSED(sourceParent)
  Q_UNUSED(sourceStart)
  Q_UNUSED(sourceEnd)
  Q_UNUSED(destinationParent)
  Q_UNUSED(destinationColumn)
  // TODO: more efficiently calculate the index range and call moved etc.
_mdlReset();
}

void QmlSectionProxyModel::srcRowsRemoved ( const QModelIndex & parent, int start, int end )
{
  Q_UNUSED(parent)
  Q_UNUSED(start)
  Q_UNUSED(end)

  // TODO: more efficiently calculate the index range and call remove etc.
_mdlReset();
}

void QmlSectionProxyModel::srcDataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
  if(topLeft.parent() == bottomRight.parent()) {
    QModelIndex tl = mapFromSource(topLeft);
    QModelIndex br = mapFromSource(bottomRight);
    emit dataChanged(tl,br);
  } else {
    qWarning() << "Handling unexpected case in QmlSectionProxyModel::srcDataChanged!!";
    _mdlReset();
  }
}
