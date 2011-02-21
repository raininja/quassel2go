#ifndef INPUTNICKPRESENTER_H
#define INPUTNICKPRESENTER_H

#include "abstractitempresenter.h"
#include "buffermodel.h"
#include "bufferinfo.h"
#include "identity.h"
#include "network.h"
#include <action.h>

class QComboBox;

class InputNickPresenter : public AbstractItemPresenter
{
    Q_OBJECT
public:
    explicit InputNickPresenter(QComboBox *ownNick, QObject *parent = 0);
  virtual ~InputNickPresenter();

  const Network *currentNetwork() const;

protected slots:
  virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
  virtual void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
  virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private slots:
  void setShowNickSelector(const QVariant &);
  void changeNick(const QString &newNick) const;

  void setNetwork(NetworkId networkId);
  void setIdentity(IdentityId identityId);
  void connectMyIrcUser();
  void updateNickSelector() const;
  void updateEnabledState();

  BufferInfo currentBufferInfo() const;

private:
  QComboBox *_ownNick;

  NetworkId _networkId;
  IdentityId _identityId;
  QMenu *_colorMenu, *_colorFillMenu;
};

#endif // INPUTNICKPRESENTER_H
