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

#include <QFile>
#include <QFileInfo>

IQConfig::IQConfig(const QString &category_, const QString &fileName_)
    : category{category_.isEmpty() ? "" : category_ + '/'}, fileName{fileName_},
      settings{std::make_unique<QSettings>(getConfigFileName(),
					   QSettings::IniFormat)}
{
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

QString IQConfig::configDir()
{
	static auto configDir = XdgDirs::configHome() + '/' + applicationName();
	return configDir;
}

QString IQConfig::applicationVersion()
{
	return QStringLiteral(IQ_MACRO_STRING(IQ_VERSION));
}
#undef IQ_MACRO_STRING__
#undef IQ_MACRO_STRING

QString IQConfig::getConfigFileName() const
{
	auto config = configDir() + '/' + fileName;
	QFileInfo config_file{config};

	if (config_file.exists()) {
		if (!config_file.isFile())
			throw std::runtime_error{config.toStdString() +
						 " is not a valid config file"};
	} else {
		copyConfigFileFromExample(config);
	}
	return config;
}

bool IQConfig::copyConfigFileFromExample(const QString &destination) const
{
	auto config_example_path =
	    "/usr/share/" + applicationName() + '/' + fileName + ".example";
	QFile config_example_file{config_example_path};
	if (!config_example_file.exists())
		return false;
	return config_example_file.copy(destination);
}

IQConfigurable::IQConfigurable(const QString &name)
    : name_{name}, config{name_, "config"}
{
}

const QString &IQConfigurable::name() const { return name_; }

bool IQConfigurable::isEnabled() const
{
	return config.value("enabled", false).toBool();
}
