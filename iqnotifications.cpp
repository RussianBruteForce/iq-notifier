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

#include <experimental/optional>
#include <memory>
#include <utility>

#include <QString>
#include <QStringList>
#include <QTimer>

template <class T> using optional = std::experimental::optional<T>;

IQNotifications::IQNotifications(IQDisposition::ptr_t disposition_,
				 QObject *parent)
    : IQNotificationReceiver(parent), IQConfigurable{"ui"},
      disposition{std::move(disposition_)}
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
	return config.value(CONFIG_SPACING, CONFIG_SPACING_DEFAULT).toInt();
}

QMargins IQNotifications::margins() const
{
	static auto string_list_to_margins =
	    [](QStringList list) -> optional<QMargins> {
		enum Sides { LEFT = 0, TOP, RIGHT, BOTTOM, SIZE };

		if (list.size() != SIZE)
			return {};

		auto get = [&list](auto index) {
			bool ok{true};
			auto value = list[index].toInt(&ok);
			if (!ok)
				throw std::logic_error{"IQConfig: "
						       "ui::global_margins "
						       "wrong value!"};
			return value;
		};

		QMargins ret;
		ret.setLeft(get(LEFT));
		ret.setTop(get(TOP));
		ret.setRight(get(RIGHT));
		ret.setBottom(get(BOTTOM));
		return {ret};
	};

	auto config_field = config.value(CONFIG_GLOBAL_MARGINS);
	auto config_margin =
	    string_list_to_margins(config_field.toStringList());
	if (config_margin) {
		return *config_margin;
	} else {
		auto screen = disposition->screen()->availableSize();
		auto margin = GLOBAL_MARGINS_DEFAULT_FACTOR * screen.height();
		auto m = static_cast<int>(margin);
		return {m, m, m, m};
	}
}

QSize IQNotifications::windowSize() const
{
	return windowSize(CONFIG_WIDTH, CONFIG_HEIGHT, WIDTH_DEFAULT_FACTOR,
			  HEIGHT_DEFAULT_FACTOR);
}

QSize IQNotifications::windowSize(const QString &width_key,
				  const QString &height_key,
				  double width_factor,
				  double height_factor) const
{
	// TODO: cache values

	auto get_window_size_from_config =
	    [width_key, height_key](const auto &config) -> optional<QSize> {
		auto get = [&config](auto key) {
			bool ok{true};
			auto value = config.value(key, 0).toInt(&ok);
			if (!ok || value < 0)
				throw std::logic_error{"IQConfig: window or "
						       "extra_window size "
						       "wrong value!"};
			return value;
		};

		auto w = get(width_key);
		auto h = get(height_key);

		if (!w || !h)
			return {};
		else
			return {QSize{w, h}};
	};

	auto config_size = get_window_size_from_config(config);
	if (config_size) {
		return *config_size;
	} else {
		auto screen = disposition->screen()->availableSize();
		auto w = width_factor * screen.width();
		auto h = height_factor * screen.height();
		return QSize{static_cast<int>(w), static_cast<int>(h)};
	}
}

QSize IQNotifications::extraWindowSize() const
{
	return windowSize(CONFIG_EXTRA_WINDOW_WIDTH, CONFIG_EXTRA_WINDOW_HEIGHT,
			  EXTRA_WINDOW_WIDTH_DEFAULT_FACTOR,
			  EXTRA_WINDOW_HEIGHT_DEFAULT_FACTOR);
}

QPoint IQNotifications::extraWindowPos() const
{
	return disposition->externalWindowPos();
}

bool IQNotifications::createNotificationIfSpaceAvailable(
    const IQNotification &n)
{
	auto size = windowSize();
	auto pos = disposition->poses(n.id, size);
	if (pos) {
		auto id = n.replaces_id ? n.replaces_id : n.id;
		emit createNotification(static_cast<int>(id), size, *pos,
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
