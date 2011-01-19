#ifndef MAEMO5NOTIFICATION_H
#define MAEMO5NOTIFICATION_H

#include <QtCore>
#include <QApplication>

class Maemo5Notification
{
public:

    static const int EXPIRES_NEVER;

    static const int DEFAULT_TIMEOUT;

    static const QString DEFAULT_ICON;

    static const QString APPLICATION_ICON_SETTING_KEY;

    static const QString applicationIcon();

    static bool information(const QString &title, const QString &text, const QString &icon, int timeout);

    static bool information(const QString &title, const QString &text);

    static bool information(const QString &text, int timeout);

    static bool information(const QString &text);
};

#endif // MAEMO5NOTIFICATION_H
