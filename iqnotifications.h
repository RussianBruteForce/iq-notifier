/*
 *     This file is part of IQ Notifier.
 *
 * IQ Notifier is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * IQ Notifier is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IQ Notifier.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <queue>

#include <QObject>
#include <QPoint>
#include <QSize>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#include <gsl/gsl>
#pragma clang diagnostic pop

#include "iqdisposition.h"
#include "iqnotificationreceiver.h"

class IQNotifications final : public IQNotificationReceiver
{
	Q_OBJECT
	Q_PROPERTY(int extraNotifications READ extraNotificationsCount NOTIFY
		       extraNotificationsCountChanged)
	Q_PROPERTY(QSize extraWindowSize READ extraWindowSize)
	Q_PROPERTY(QPoint extraWindowPos READ extraWindowPos)

	IQNotifications(IQDisposition::ptr_t disposition_,
			QObject *parent = nullptr);

      public:
	static gsl::not_null<IQNotifications *>
	get(IQDisposition::ptr_t disposition = nullptr);

	QSize extraWindowSize() const;
	QPoint extraWindowPos() const;
	int extraNotificationsCount() const;

      signals:
	// Signals to QML
	void extraNotificationsCountChanged();
	void createNotification(int notification_id, QSize size, QPoint pos,
				int expire_timeout, const QString &appName,
				const QString &body,
				const QString &title = QString{},
				const QString &iconUrl = QString{},
				const QStringList &actions = {});
	void dropNotification(int notification_id);
	void dropAllVisible();
	void moveNotification(int notification_id, QPoint pos);

      public slots:
	void onCreateNotification(const IQNotification &notification) final;
	void onDropNotification(IQNotification::id_t id) final;

	// QML slots
	void onCloseButtonPressed(int id);
	void onActionButtonPressed(int id, const QString &action);
	void onExpired(int id);
	void onDropAll();
	void onDropStacked();
	void onDropVisible();

      private:
	IQDisposition::ptr_t disposition;
	std::queue<IQNotification> extraNotifications;

	int spacing() const;
	QMargins margins() const;
	bool createNotificationIfSpaceAvailable(const IQNotification &n);
	void checkExtraNotifications();
};
