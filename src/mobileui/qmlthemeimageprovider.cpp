#include <QIcon>
#include "qmlthemeimageprovider.h"
#include "iconloader.h"
#include <QDebug>

QmlThemeImageProvider::QmlThemeImageProvider(QObject *parent) :
  QObject(parent),
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
}

QPixmap QmlThemeImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
  QPixmap pm = QIcon::fromTheme(id).pixmap(requestedSize);
  if(pm.isNull()) {
    pm = DesktopIcon(id, qMax(requestedSize.height(),requestedSize.width()));
  }
  if(pm.isNull()) {
    pm = DesktopIcon(id);
  }
  if(pm.isNull()) {
    pm = SmallIcon(id);
  }
  if(pm.isNull()) {
    pm = BarIcon(id);
  }
  if(pm.isNull()) {
    pm = MainBarIcon(id);
  }

  if (size)
    *size = pm.size();
  return pm;
}
