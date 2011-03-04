#define QT_NO_KEYWORDS

#include "maemo5notification.h"
#if defined(Q_WS_MAEMO_5) || defined(Q_WS_HILDON)
#include <libnotify/notify.h>
#endif

#include <QtGui>

#if defined(Q_WS_MAEMO_5) || defined(Q_WS_HILDON)
const int Maemo5Notification::EXPIRES_NEVER = NOTIFY_EXPIRES_NEVER;

const int Maemo5Notification::DEFAULT_TIMEOUT = NOTIFY_EXPIRES_DEFAULT;
#endif

const QString Maemo5Notification::DEFAULT_ICON("general_web");

const QString Maemo5Notification::APPLICATION_ICON_SETTING_KEY("Maemo5Notification_application_icon");


const QString Maemo5Notification::applicationIcon()
{
  return "quassel";
}

bool Maemo5Notification::information(const QString &title, const QString &text, const QString &icon, int timeout)
{
#if defined(Q_WS_MAEMO_5) || defined(Q_WS_HILDON)
    bool successful = true;
    notify_init(title.toLatin1().data());
    NotifyNotification *notification = notify_notification_new(title.toLatin1().data(), text.toLatin1().data(), icon.toLatin1().data(), NULL);

    if (notification == NULL)
        return false;

    notify_notification_set_timeout(notification, timeout);

    if (!notify_notification_show(notification, NULL))
        successful = false;

    g_object_unref(notification);

    return successful;
#else
    Q_UNUSED(title)
    Q_UNUSED(text)
    Q_UNUSED(icon)
    Q_UNUSED(timeout)
    return false;
#endif
}

bool Maemo5Notification::information(const QString &title, const QString &text)
{
    return information(title, text, DEFAULT_ICON, DEFAULT_TIMEOUT);
}

bool Maemo5Notification::information(const QString &text, int timeout)
{
    return information(QApplication::applicationName(), text, applicationIcon(), timeout);
}

bool Maemo5Notification::information(const QString &text)
{
    return information(QApplication::applicationName(), text, applicationIcon(), DEFAULT_TIMEOUT);
}
