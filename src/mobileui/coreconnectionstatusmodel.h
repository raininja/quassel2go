#ifndef CORECONNECTIONSTATUSMODEL_H
#define CORECONNECTIONSTATUSMODEL_H

#include <QObject>

#include "coreconnection.h"

class CoreConnectionStatusModel : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString connectionIconName READ connectionIconName NOTIFY connectionChanged)
  Q_PROPERTY(QString connectionString READ connectionString NOTIFY connectionChanged)
  Q_PROPERTY(QString lagString READ lagString NOTIFY lagChanged)

public:
  explicit CoreConnectionStatusModel(CoreConnection *connection, QObject *parent = 0);

  inline CoreConnection *coreConnection() const { return _coreConnection; }

  QString connectionString() const;
  QString connectionIconName() const;
  QString lagString() const;

signals:
  void connectionChanged();
  void lagChanged();
  void connectionErrorChanged();

private slots:
  void updateLag(int msecs);

private:
  CoreConnection *_coreConnection;
  int _lagMsecs;
};

#endif // CORECONNECTIONSTATUSMODEL_H
