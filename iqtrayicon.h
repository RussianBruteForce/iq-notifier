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

#include <QObject>
#include <QSystemTrayIcon>
#include <QUrl>

class IQTrayIcon : public QSystemTrayIcon
{
	Q_OBJECT
	Q_PROPERTY(
	    QUrl iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
      public:
	explicit IQTrayIcon(QObject *parent = nullptr);

	QUrl iconUrl() const;
	void setIconUrl(const QUrl &iconUrl);

      signals:
	void iconUrlChanged();
	void leftClick();

      private:
	QUrl iconUrl_;
};
