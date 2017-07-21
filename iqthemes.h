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

#include <QColor>
#include <QObject>
#include <QUrl>

#include "iqconfig.h"

class NotificationsTheme;

class IQThemes : public QObject
{
	Q_OBJECT
	Q_PROPERTY(NotificationsTheme *notificationsTheme READ
		       notificationsTheme CONSTANT)

      public:
	IQThemes();

	NotificationsTheme *notificationsTheme() const;

      private:
	IQ_CONF_VAR(THEME_NAME, "theme_name", "default")

	IQConfig config;
	const QString themeName;
	std::shared_ptr<IQConfig> themeConfig;
	std::unique_ptr<NotificationsTheme> notificationsTheme_;

	QString themeConfigDir() const;
	QString themeConfigFile() const;
	void loadTheme(const QString &fileName);
	void registerThemeTypes() const;
};

class IQTheme : public QObject
{
	Q_OBJECT
      public:
	IQTheme() = default;
	IQTheme(const std::shared_ptr<IQConfig> &config_,
		const QString &themeDir_, QObject *parent = nullptr);

      protected:
	std::shared_ptr<IQConfig> themeConfig;
	const QString themeDir;

	QUrl toRelativeUrl(const QString &str) const;
};

class NotificationsTheme : public IQTheme
{
	Q_OBJECT

	Q_PROPERTY(bool iconPosition READ iconPosition CONSTANT)
	Q_PROPERTY(uint fontSize READ fontSize CONSTANT)
	Q_PROPERTY(uint barFontSize READ barFontSize CONSTANT)
	Q_PROPERTY(uint iconSize READ iconSize CONSTANT)
	Q_PROPERTY(uint barHeight READ barHeight CONSTANT)
	Q_PROPERTY(uint expirationBarHeight READ expirationBarHeight CONSTANT)
	Q_PROPERTY(
	    uint showAnimationDuration READ showAnimationDuration CONSTANT)
	Q_PROPERTY(
	    uint dropAnimationDuration READ dropAnimationDuration CONSTANT)
	Q_PROPERTY(
	    double closeButtonImageScale READ closeButtonImageScale CONSTANT)
	Q_PROPERTY(
	    double extraButtonImageScale READ extraButtonImageScale CONSTANT)
	Q_PROPERTY(QColor bgColor READ bgColor CONSTANT)
	Q_PROPERTY(QColor barBgColor READ barBgColor CONSTANT)
	Q_PROPERTY(QColor barTextColor READ barTextColor CONSTANT)
	Q_PROPERTY(QColor expirationBarColor READ expirationBarColor CONSTANT)
	Q_PROPERTY(QColor titleTextColor READ titleTextColor CONSTANT)
	Q_PROPERTY(QColor bodyTextColor READ bodyTextColor CONSTANT)
	Q_PROPERTY(QColor buttonBgColor READ buttonBgColor CONSTANT)
	Q_PROPERTY(QColor buttonTextColor READ buttonTextColor CONSTANT)
	Q_PROPERTY(QColor extraBgColor READ extraBgColor CONSTANT)
	Q_PROPERTY(
	    QColor extraUreadCircleColor READ extraUreadCircleColor CONSTANT)
	Q_PROPERTY(QColor extraUreadTextColor READ extraUreadTextColor CONSTANT)
	Q_PROPERTY(QUrl bgImage READ bgImage CONSTANT)
	Q_PROPERTY(QUrl closeButtonImage READ closeButtonImage CONSTANT)
	Q_PROPERTY(
	    QUrl extraCloseButtonImage READ extraCloseButtonImage CONSTANT)
	Q_PROPERTY(QUrl extraCloseAllButtonImage READ extraCloseAllButtonImage
		       CONSTANT)
	Q_PROPERTY(QUrl extraCloseVisibleButtonImage READ
		       extraCloseVisibleButtonImage CONSTANT)

      public:
	using IQTheme::IQTheme;

	// True to move to left side, false for top
	bool iconPosition() const;
	uint fontSize() const;
	uint barFontSize() const;
	uint iconSize() const;
	uint barHeight() const;
	uint expirationBarHeight() const;
	uint showAnimationDuration() const;
	uint dropAnimationDuration() const;
	double closeButtonImageScale() const;
	double extraButtonImageScale() const;
	QColor bgColor() const;
	QColor barBgColor() const;
	QColor barTextColor() const;
	QColor expirationBarColor() const;
	QColor titleTextColor() const;
	QColor bodyTextColor() const;
	QColor buttonBgColor() const;
	QColor buttonTextColor() const;
	QColor extraBgColor() const;
	QColor extraUreadCircleColor() const;
	QColor extraUreadTextColor() const;
	QUrl bgImage() const;
	QUrl closeButtonImage() const;
	QUrl extraCloseButtonImage() const;
	QUrl extraCloseAllButtonImage() const;
	QUrl extraCloseVisibleButtonImage() const;

      private:
	IQ_CONF_VAR(ICON_POSITION, "popup_notifications/icon_position", "top")

	IQ_CONF_VAR(FONT_SIZE, "popup_notifications/font_size", 0)
	IQ_CONF_VAR(BAR_FONT_SIZE, "popup_notifications/bar_font_size", 0)
	IQ_CONF_VAR(ICON_SIZE, "popup_notifications/icon_size", 0)
	IQ_CONF_VAR(BAR_HEIGHT, "popup_notifications/bar_height", 0)
	IQ_CONF_VAR(EXPIRATION_BAR_HEIGHT,
		    "popup_notifications/expiration_bar_height", 0)
	IQ_CONF_VAR(SHOW_DURATION,
		    "popup_notifications/show_animation_duration", 120)
	IQ_CONF_VAR(DROP_DURATION,
		    "popup_notifications/drop_animation_duration", 120)
	IQ_CONF_VAR(CLOSE_BUTTON_IMAGE_SCALE,
		    "popup_notifications/close_button_image_scale", 0.4)
	IQ_CONF_VAR(EXTRA_BUTTON_IMAGE_SCALE,
		    "popup_notifications/extra_button_image_scale", 0.6)

	IQ_CONF_VAR(BG_COLOR, "popup_notifications/bg_color", "#19202d")
	IQ_CONF_VAR(BAR_BG_COLOR, "popup_notifications/bar_bg_color", "#262d3a")
	IQ_CONF_VAR(BAR_TEXT_COLOR, "popup_notifications/bar_text_color",
		    "#92969c")
	IQ_CONF_VAR(EXPIRATION_BAR_COLOR,
		    "popup_notifications/expiration_bar_color", "#30394a")
	IQ_CONF_VAR(TITLE_TEXT_COLOR, "popup_notifications/title_text_color",
		    "#ffffff")
	IQ_CONF_VAR(BODY_TEXT_COLOR, "popup_notifications/body_text_color",
		    "#92969c")
	IQ_CONF_VAR(BUTTON_BG_COLOR, "popup_notifications/button_bg_color",
		    "#343b4d")
	IQ_CONF_VAR(BUTTON_TEXT_COLOR, "popup_notifications/button_text_color",
		    "#ffffff")
	IQ_CONF_VAR(EXTRA_BG_COLOR, "popup_notifications/extra_bg_color",
		    "#262d3a")
	IQ_CONF_VAR(EXTRA_UNREAD_CIRCLE_COLOR,
		    "popup_notifications/extra_unread_circle_color", "#d74a37")
	IQ_CONF_VAR(EXTRA_UNREAD_TEXT_COLOR,
		    "popup_notifications/extra_unread_text_color", "#ffffff")

	IQ_CONF_VAR(BG_IMAGE, "popup_notifications/bg_image", "")
	IQ_CONF_VAR(CLOSE_BUTTON_IMAGE,
		    "popup_notifications/close_button_image", "img/close.png")
	IQ_CONF_VAR(EXTRA_CLOSE_BUTTON_IMAGE,
		    "popup_notifications/extra_close_button_image",
		    "img/close.png")
	IQ_CONF_VAR(EXTRA_CLOSE_ALL_BUTTON_IMAGE,
		    "popup_notifications/extra_close_all_button_image",
		    "img/closeAll.png")
	IQ_CONF_VAR(EXTRA_CLOSE_VISIBLE_BUTTON_IMAGE,
		    "popup_notifications/extra_close_visible_button_image",
		    "img/closeVisible.png")
};
