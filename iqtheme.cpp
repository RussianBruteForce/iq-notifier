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

#include "iqtheme.h"

IQTheme::IQTheme()
    : config{"general"},
      themeName{
	  config.value(CONFIG_THEME_NAME, CONFIG_THEME_NAME_DEFAULT).toString()}
{
	loadTheme(themeConfigFile());
}

bool IQTheme::iconPosition() const
{
	auto pos =
	    theme->value(CONFIG_ICON_POSITION, CONFIG_ICON_POSITION_DEFAULT)
		.toString();
	return pos.compare("left", Qt::CaseInsensitive) == 0;
}

uint IQTheme::fontSize() const
{
	return theme->value(CONFIG_FONT_SIZE, CONFIG_FONT_SIZE_DEFAULT)
	    .toUInt();
}

uint IQTheme::iconSize() const
{
	return theme->value(CONFIG_ICON_SIZE, CONFIG_ICON_SIZE_DEFAULT)
	    .toUInt();
}

uint IQTheme::barHeight() const
{
	return theme->value(CONFIG_BAR_HEIGHT, CONFIG_BAR_HEIGHT_DEFAULT)
	    .toUInt();
}

uint IQTheme::expirationBarHeight() const
{
	return theme
	    ->value(CONFIG_EXPIRATION_BAR_HEIGHT,
		    CONFIG_EXPIRATION_BAR_HEIGHT_DEFAULT)
	    .toUInt();
}

QString IQTheme::themeConfigFile() const
{
	return "themes/" + themeName + "/theme";
}

void IQTheme::loadTheme(const QString &fileName)
{
	theme = std::make_unique<IQConfig>(QString{}, fileName);
}
