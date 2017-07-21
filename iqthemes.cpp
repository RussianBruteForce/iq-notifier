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

#include "iqthemes.h"

#include <QtQml/qqml.h>

IQThemes::IQThemes()
    : config{"theme"},
      themeName{
	  config.value(CONFIG_THEME_NAME, CONFIG_THEME_NAME_DEFAULT).toString()}
{
	registerThemeTypes();
	loadTheme(themeConfigFile());
	auto themeDir = IQConfig::configDir() + '/' + themeConfigDir();
	notificationsTheme_ =
	    std::make_unique<NotificationsTheme>(themeConfig, themeDir);
}

NotificationsTheme *IQThemes::notificationsTheme() const
{
	return notificationsTheme_.get();
}

QString IQThemes::themeConfigDir() const { return "themes/" + themeName; }

QString IQThemes::themeConfigFile() const
{
	return themeConfigDir() + "/theme";
}

void IQThemes::loadTheme(const QString &fileName)
{
	themeConfig = std::make_shared<IQConfig>(QString{}, fileName);
}

void IQThemes::registerThemeTypes() const
{
	qmlRegisterType<NotificationsTheme>("IQNotifier", 1, 0,
					    "NotificationsTheme");
}

IQTheme::IQTheme(const std::shared_ptr<IQConfig> &config_,
		 const QString &themeDir_, QObject *parent)
    : QObject(parent), themeConfig{config_}, themeDir{themeDir_}
{
}

QUrl IQTheme::toRelativeUrl(const QString &str) const
{
	return "file:///" + themeDir + '/' + str;
}

/*
 *
 * THEMES NEXT
 *
 */

#define IQ_THEME_UINT(KEY__) themeConfig->value(KEY__, KEY__##_DEFAULT).toUInt()

#define IQ_THEME_DOUBLE(KEY__)                                                 \
	themeConfig->value(KEY__, KEY__##_DEFAULT).toDouble()

#define IQ_THEME_STRING(KEY__)                                                 \
	themeConfig->value(KEY__, KEY__##_DEFAULT).toString()

#define IQ_THEME_COLOR(KEY__) IQ_THEME_STRING(KEY__)

#define IQ_THEME_IMAGE(KEY__)                                                  \
	auto str = IQ_THEME_STRING(KEY__);                                     \
	if (str == KEY__##_DEFAULT)                                            \
		return str;                                                    \
	auto url = toRelativeUrl(str);                                         \
	if (url.isValid())                                                     \
		return url;                                                    \
	else                                                                   \
		return QString{KEY__##_DEFAULT};

bool NotificationsTheme::iconPosition() const
{
	auto pos =
	    themeConfig
		->value(CONFIG_ICON_POSITION, CONFIG_ICON_POSITION_DEFAULT)
		.toString();
	return pos.compare("left", Qt::CaseInsensitive) == 0;
}

uint NotificationsTheme::fontSize() const
{
	return IQ_THEME_UINT(CONFIG_FONT_SIZE);
}

uint NotificationsTheme::barFontSize() const
{
	return IQ_THEME_UINT(CONFIG_BAR_FONT_SIZE);
}

uint NotificationsTheme::iconSize() const
{
	return IQ_THEME_UINT(CONFIG_ICON_SIZE);
}

uint NotificationsTheme::barHeight() const
{
	return IQ_THEME_UINT(CONFIG_BAR_HEIGHT);
}

uint NotificationsTheme::expirationBarHeight() const
{
	return IQ_THEME_UINT(CONFIG_EXPIRATION_BAR_HEIGHT);
}

uint NotificationsTheme::showAnimationDuration() const
{
	return IQ_THEME_UINT(CONFIG_SHOW_DURATION);
}

uint NotificationsTheme::dropAnimationDuration() const
{
	return IQ_THEME_UINT(CONFIG_DROP_DURATION);
}

double NotificationsTheme::closeButtonImageScale() const
{
	return IQ_THEME_DOUBLE(CONFIG_CLOSE_BUTTON_IMAGE_SCALE);
}

double NotificationsTheme::extraButtonImageScale() const
{
	return IQ_THEME_DOUBLE(CONFIG_EXTRA_BUTTON_IMAGE_SCALE);
}

QColor NotificationsTheme::bgColor() const
{
	return IQ_THEME_COLOR(CONFIG_BG_COLOR);
}

QColor NotificationsTheme::barBgColor() const
{
	return IQ_THEME_COLOR(CONFIG_BAR_BG_COLOR);
}

QColor NotificationsTheme::barTextColor() const
{
	return IQ_THEME_COLOR(CONFIG_BAR_TEXT_COLOR);
}

QColor NotificationsTheme::expirationBarColor() const
{
	return IQ_THEME_COLOR(CONFIG_EXPIRATION_BAR_COLOR);
}

QColor NotificationsTheme::titleTextColor() const
{
	return IQ_THEME_COLOR(CONFIG_TITLE_TEXT_COLOR);
}

QColor NotificationsTheme::bodyTextColor() const
{
	return IQ_THEME_COLOR(CONFIG_BODY_TEXT_COLOR);
}

QColor NotificationsTheme::buttonBgColor() const
{
	return IQ_THEME_COLOR(CONFIG_BUTTON_BG_COLOR);
}

QColor NotificationsTheme::buttonTextColor() const
{
	return IQ_THEME_COLOR(CONFIG_BUTTON_TEXT_COLOR);
}

QColor NotificationsTheme::extraBgColor() const
{
	return IQ_THEME_COLOR(CONFIG_EXTRA_BG_COLOR);
}

QColor NotificationsTheme::extraUreadCircleColor() const
{
	return IQ_THEME_COLOR(CONFIG_EXTRA_UNREAD_CIRCLE_COLOR);
}

QColor NotificationsTheme::extraUreadTextColor() const
{
	return IQ_THEME_COLOR(CONFIG_EXTRA_UNREAD_TEXT_COLOR);
}

QUrl NotificationsTheme::bgImage() const { IQ_THEME_IMAGE(CONFIG_BG_IMAGE) }

QUrl NotificationsTheme::closeButtonImage() const
{
	IQ_THEME_IMAGE(CONFIG_CLOSE_BUTTON_IMAGE)
}

QUrl NotificationsTheme::extraCloseButtonImage() const
{
	IQ_THEME_IMAGE(CONFIG_EXTRA_CLOSE_BUTTON_IMAGE)
}

QUrl NotificationsTheme::extraCloseAllButtonImage() const
{
	IQ_THEME_IMAGE(CONFIG_EXTRA_CLOSE_ALL_BUTTON_IMAGE)
}

QUrl NotificationsTheme::extraCloseVisibleButtonImage() const
{
	IQ_THEME_IMAGE(CONFIG_EXTRA_CLOSE_VISIBLE_BUTTON_IMAGE)
}

#undef IQ_THEME_STRING
#undef IQ_THEME_COLOR
#undef IQ_THEME_DOUBLE
#undef IQ_THEME_UINT
