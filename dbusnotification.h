#pragma once

#include <QString>
#include <QStringList>
#include <QVariantMap>

struct DBusNotification {
	using id_t = uint;

	enum ClosingReason : uint {
		CR_NOTIFICATION_EXPIRED = 1,
		CR_NOTIFICATION_DISMISSED,
		CR_NOTIFICATION_CLOSED,
		CR_NOTIFICATION_CLOSED_REASON_UNDEFINED
	};

	enum ExpireTimeout : int { ET_SERVER_DECIDES = -1, ET_FOREVER = 0 };

	id_t id;
	QString app_name;
	id_t replaces_id;
	QString app_icon;
	QString summary;
	QString body;
	QStringList actions;
	QVariantMap hints;
	ExpireTimeout expire_timeout;
};
