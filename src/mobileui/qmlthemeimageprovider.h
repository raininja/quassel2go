#ifndef QMLTHEMEIMAGEPROVIDER_H
#define QMLTHEMEIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

class QmlThemeImageProvider : public QObject, public QDeclarativeImageProvider
{
    Q_OBJECT
public:
    explicit QmlThemeImageProvider(QObject *parent = 0);

  QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

signals:

public slots:

};

#endif // QMLTHEMEIMAGEPROVIDER_H
