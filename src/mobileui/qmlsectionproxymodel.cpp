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

    // always map to the only column

    int cnt = 0;
    bool success = false;
    for(int i = 0; i < sourceModel()->rowCount(_root); ++i) {
      QModelIndex sectionIndex = sourceModel()->index(i, 0, _root);

      if(sectionIndex == sourceIndex.parent()) {
        cnt += sourceIndex.row();
        success = true;
        break;
      } else {
        cnt += sourceModel()->rowCount(sectionIndex);
      }
    }

    if(success)
      return index(cnt, 0, _root);
    else
      return QModelIndex();
}

QModelIndex QmlSectionProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if(!sourceModel())
        return QModelIndex();

    // always map to first column

    int row = proxyIndex.row();

    int cnt = 0;
    for(int i = 0; i < sourceModel()->rowCount(_root); ++i) {
      QModelIndex sectionIndex = sourceModel()->index(i, 0, _root);
      int sectionCount = sourceModel()->rowCount(sectionIndex);

      if(row >= cnt-1 && row <= (cnt+sectionCount-1)) {
        return sourceModel()->index(row-cnt, 0, sectionIndex);
      }

      cnt += sectionCount;
    }

    return QModelIndex();
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

    int cnt = 0;
    for(int i = 0; i < sourceModel()->rowCount(_root); ++i) {
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
    setRoleNames(roles);

    connect(sourceModel, SIGNAL(modelReset()), this, SLOT(_mdlReset()));
    connect(sourceModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(_mdlReset()));
    connect(sourceModel, SIGNAL(rowsMoved(const QModelIndex &,int,int, const QModelIndex &, int)), this, SLOT(_mdlReset()));
    connect(sourceModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(_mdlReset()));
  }
  _root = QModelIndex();

  reset();

  endResetModel();/*
  reset();*/
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
    if(role == Qt::UserRole+999) {
      return sourceModel()->data(mapToSource(proxyIndex).parent(),
                                 Qt::DisplayRole);
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

    int cnt = 0;
    for(int i = 0; i < sourceModel()->rowCount(_root); ++i) {
      QModelIndex sectionIndex = sourceModel()->index(i, 0, _root);
      int sectionCount = sourceModel()->rowCount(sectionIndex);

      if(row >= cnt-1 && row <= (cnt+sectionCount-1)) {
        return createIndex(row, column, sourceModel()->index(row-(cnt-1), column, sectionIndex).internalPointer());
      }

      cnt += sectionCount;
    }

    return QModelIndex();
}

void QmlSectionProxyModel::setSourceRootIndex(const QModelIndex &root)
{
  if(_root == root)
    return;

  _root = root;
  reset();
}
