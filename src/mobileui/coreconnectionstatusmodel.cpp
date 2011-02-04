#include "client.h"
#include "iconloader.h"
#include "signalproxy.h"

#include "coreconnectionstatusmodel.h"

CoreConnectionStatusModel::CoreConnectionStatusModel(CoreConnection *connection, QObject *parent) :
    QObject(parent),
  _coreConnection(connection),
  _lagMsecs(0)
{
  connect(coreConnection(), SIGNAL(stateChanged(CoreConnection::ConnectionState)), SIGNAL(connectionChanged()));
  connect(coreConnection(), SIGNAL(lagUpdated(int)), SLOT(updateLag(int)));
  connect(coreConnection(), SIGNAL(lagUpdated(int)), SIGNAL(lagChanged()));
}

void CoreConnectionStatusModel::updateLag(int msecs) {
  _lagMsecs = msecs;
}

QString CoreConnectionStatusModel::lagString() const
{
  QString unit = _lagMsecs >= 100 ? tr("s", "seconds") : tr("ms", "milliseconds");
  return tr("(Lag: %1 %2)").arg(_lagMsecs >= 100 ? _lagMsecs / 1000. : _lagMsecs, 0, 'f', (int)(_lagMsecs >= 100)).arg(unit);
}

QString CoreConnectionStatusModel::connectionString() const
{
  if (coreConnection()->state() != CoreConnection::Connected)
    return "";

  if(coreConnection()->isEncrypted()) {
    return "security-high";
  } else {
    return "security-low";
  }
}

QString CoreConnectionStatusModel::connectionIconName() const
{
  if (coreConnection()->state() != CoreConnection::Connected)
    return "";

  if(coreConnection()->isEncrypted()) {
    return tr("The connection to your core is encrypted with SSL.");
  } else {
    return tr("The connection to your core is not encrypted.");
  }
}
