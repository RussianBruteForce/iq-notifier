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

#include "iqnotifications.h"

#include <memory>
#include <utility>

#include <QTimer>

IQNotifications::IQNotifications(IQDisposition::ptr_t disposition_,
				 QObject *parent)
    : IQNotificationReceiver(parent), disposition{std::move(disposition_)}
{
	if (!disposition)
		throw std::invalid_argument{
		    "IQNotifications: provide disposition"};
	disposition->setExtraWindowSize(extraWindowSize());
	disposition->setSpacing(spacing());
	disposition->setMargins(margins());

	connect(this, &IQNotifications::dropNotification, disposition.get(),
		&IQDisposition::remove);
	connect(disposition.get(), &IQDisposition::moveNotification,
		[this](IQNotification::id_t id, QPoint pos) {
			emit moveNotification(static_cast<int>(id), pos);
			checkExtraNotifications();
		});
}

gsl::not_null<IQNotifications *>
IQNotifications::get(IQDisposition::ptr_t disposition)
{
	static IQNotifications *ptr_{nullptr};
	if (!ptr_) {
		if (!disposition)
			throw std::invalid_argument("IQDisposition: call get "
						    "with valid disposition "
						    "first");

		ptr_ = new IQNotifications{std::move(disposition)};
	}
	return {ptr_};
}

int IQNotifications::extraNotificationsCount() const
{
	return static_cast<int>(extraNotifications.size());
}

void IQNotifications::onCreateNotification(const IQNotification &n)
{
	if (!createNotificationIfSpaceAvailable(n)) {
		extraNotifications.push(n);
		emit extraNotificationsCountChanged();
	}
}

void IQNotifications::onDropNotification(IQNotification::id_t id)
{
	emit dropNotification(static_cast<int>(id));
	emit notificationDroppedSignal(id,
				       IQNotification::CR_NOTIFICATION_CLOSED);
}

void IQNotifications::onCloseButtonPressed(int id)
{
	emit dropNotification(id);
	emit notificationDroppedSignal(
	    static_cast<IQNotification::id_t>(id),
	    IQNotification::CR_NOTIFICATION_DISMISSED);
	checkExtraNotifications();
}

void IQNotifications::onActionButtonPressed(int id, const QString &action)
{
	emit dropNotification(id);
	emit actionInvokedSignal(static_cast<IQNotification::id_t>(id), action);
	emit notificationDroppedSignal(
	    static_cast<IQNotification::id_t>(id),
	    IQNotification::CR_NOTIFICATION_DISMISSED);
}

void IQNotifications::onExpired(int id)
{
	emit dropNotification(id);
	emit notificationDroppedSignal(static_cast<IQNotification::id_t>(id),
				       IQNotification::CR_NOTIFICATION_EXPIRED);
}

void IQNotifications::onDropAll()
{
	onDropStacked();
	onDropVisible();
}

void IQNotifications::onDropStacked()
{
	decltype(extraNotifications) empty;
	std::swap(extraNotifications, empty);
	emit extraNotificationsCountChanged();
}

void IQNotifications::onDropVisible()
{
	emit dropAllVisible();
	disposition->removeAll();
	checkExtraNotifications();
}

int IQNotifications::spacing() const
{
	return 0; // No spacing
}

QMargins IQNotifications::margins() const
{
	static constexpr auto MARGIN_FACTOR{0.02610966057441253264};
	auto screen = disposition->screen()->availableSize();

	auto margin = MARGIN_FACTOR * screen.height();
	auto m = static_cast<int>(margin);
	return {m, m, m, m};
}

QSize IQNotifications::extraWindowSize() const
{
	static const auto WIDTH_FACTOR{0.21961932650073206442};
	static const auto HEIGHT_FACTOR{0.08355091383812010444 / 2}; // Magic…
	auto screen = disposition->screen()->availableSize();

	auto w = WIDTH_FACTOR * screen.width();
	auto h = HEIGHT_FACTOR * screen.height();
	return QSize{static_cast<int>(w), static_cast<int>(h)};
}

QPoint IQNotifications::extraWindowPos() const
{
	return disposition->externalWindowPos();
}

bool IQNotifications::createNotificationIfSpaceAvailable(
    const IQNotification &n)
{
	// TODO: make relative to screen DPI & content
	static const QSize defaultSize{300, 216};

	auto pos = disposition->poses(n.id, defaultSize);
	if (pos) {
		auto id = n.replaces_id ? n.replaces_id : n.id;
		emit createNotification(static_cast<int>(id), defaultSize, *pos,
					n.expire_timeout, n.application, n.body,
					n.title, n.icon_url, n.actions);
		return true;
	} else {
		return false;
	}
}

void IQNotifications::checkExtraNotifications()
{
	while (!extraNotifications.empty() &&
	       createNotificationIfSpaceAvailable(extraNotifications.front())) {
		extraNotifications.pop();
		emit extraNotificationsCountChanged();
	}
}
