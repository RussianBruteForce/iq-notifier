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

#include "iqnotification.h"

namespace IQNotificationModifiers
{

template <class T, class... A> IQNotificationModifier::ptr_t make(A &&... args)
{
	return std::make_unique<T>(std::forward<A>(args)...);
}

struct IDGenerator final : public IQNotificationModifier {
	IQNotification::id_t last_id{0};

	void modify(IQNotification &notification) final;
};

struct IconHandler final : public IQNotificationModifier {
	void modify(IQNotification &notification) final;
};

struct DefaultTimeout final : public IQNotificationModifier {
	explicit DefaultTimeout(uint16_t msec);
	void modify(IQNotification &notification) final;

      private:
	uint16_t defaultTimeout;
};

struct TitleToIcon final : public IQNotificationModifier {
	void modify(IQNotification &notification) final;
};

struct BodyToTitleWhenTitleIsAppName final : public IQNotificationModifier {
	void modify(IQNotification &notification) final;
};

struct ReplaceMinusToDash final : public IQNotificationModifier {
	explicit ReplaceMinusToDash(bool title = true, bool body = true);

	void modify(IQNotification &notification) final;

      private:
	const bool fixTitle, fixBody;

	static void replaceMinusToDash(QString &str);

	static constexpr auto minusPattern{" - "};
	static constexpr auto replaceTo{" — "};
};

} // namespace IQNotificationModifiers
