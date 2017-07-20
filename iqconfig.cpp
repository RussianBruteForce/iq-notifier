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

#include "iqconfig.h"

#include <qt5xdg/XdgDirs>


IQConfig::IQConfig(const QString &category_)
    : category{category_ + '/'}, settings{std::make_unique<QSettings>(
				     getConfigFileName(), QSettings::IniFormat)}
{
	if (category.isEmpty())
		throw std::invalid_argument(
		    "IQConfig: category can not be empty!");
}

QVariant IQConfig::value(const QString &key, const QVariant &defaultValue) const
{
	return settings->value(category + key, defaultValue);
}

#define IQ_MACRO_STRING(S) IQ_MACRO_STRING__(S)
#define IQ_MACRO_STRING__(S) #S
QString IQConfig::applicationName()
{
	return QStringLiteral(IQ_MACRO_STRING(IQ_APP_NAME));
}

QString IQConfig::applicationVersion()
{
	return QStringLiteral(IQ_MACRO_STRING(IQ_VERSION));
}
#undef IQ_MACRO_STRING__
#undef IQ_MACRO_STRING

QString IQConfig::getConfigFileName()
{
	static auto config =
	    XdgDirs::configHome() + '/' + applicationName() + "/config";
	return config;
}

IQConfigurable::IQConfigurable(const QString &name) : name_{name}, config{name_}
{
}

const QString &IQConfigurable::name() const { return name_; }

bool IQConfigurable::isEnabled() const
{
	return config.value("enabled", false).toBool();
}
