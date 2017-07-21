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

#include <memory>

#include <QObject>

#include "iqconfig.h"

class IQTheme : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool iconPosition READ iconPosition CONSTANT)
	Q_PROPERTY(uint fontSize READ fontSize CONSTANT)
	Q_PROPERTY(uint iconSize READ iconSize CONSTANT)
	Q_PROPERTY(uint barHeight READ barHeight CONSTANT)
	Q_PROPERTY(uint expirationBarHeight READ expirationBarHeight CONSTANT)

      public:
	IQTheme();

	// True to move to left side, false for top
	bool iconPosition() const;
	uint fontSize() const;
	uint iconSize() const;
	uint barHeight() const;
	uint expirationBarHeight() const;

      private:
	/*
	 * Main config
	 */
	IQ_CONF_VAR(THEME_NAME, "theme", "default")

	/*
	 * Theme config
	 */
	IQ_CONF_VAR(ICON_POSITION, "popup_notifications/icon_position", "top")
	IQ_CONF_VAR(FONT_SIZE, "popup_notifications/font_size", 0)
	IQ_CONF_VAR(ICON_SIZE, "popup_notifications/icon_size", 0)
	IQ_CONF_VAR(BAR_HEIGHT, "popup_notifications/bar_height", 0)
	IQ_CONF_VAR(EXPIRATION_BAR_HEIGHT,
		    "popup_notifications/expiration_bar_height", 0)

	IQConfig config;
	const QString themeName;
	std::unique_ptr<IQConfig> theme;

	QString themeConfigFile() const;
	void loadTheme(const QString &fileName);
};
