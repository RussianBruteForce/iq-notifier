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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDBus/QDBusConnection>
#include <QtQml>

#include "notificationsadaptor.h"

#include "iqdbusservice.h"
#include "iqexpirationcontroller.h"
#include "iqnotificationmodifiers.h"
#include "iqnotifications.h"
#include "iqtopdown.h"

/*
 * Should be called first
 */
static gsl::not_null<IQDBusService *> get_service();
static QDBusConnection
connect_to_session_bus(gsl::not_null<IQDBusService *> service);
static QObject *iqnotifications_provider(QQmlEngine *engine,
					 QJSEngine *scriptEngine);

gsl::not_null<IQDBusService *> get_service()
{
	using namespace IQNotificationModifiers; // NOLINT

	auto disposition = std::make_unique<IQTopDown>();
	auto dbus_service =
	    (new IQDBusService)
		->addModifier(make<IDGenerator>())
		->addModifier(make<TitleToIcon>())
		->addModifier(make<IconHandler>())
		->addModifier(make<BodyToTitleWhenTitleIsAppName>())
		->addModifier(make<DefaultTimeout>())
		->addModifier(make<ReplaceMinusToDash>());

	auto notifications = IQNotifications::get(std::move(disposition));
	if (notifications->isEnabled())
		dbus_service->connectReceiver(notifications);

	return dbus_service;
}

QObject *iqnotifications_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine);
	Q_UNUSED(scriptEngine);
	return IQNotifications::get();
}

QDBusConnection connect_to_session_bus(gsl::not_null<IQDBusService *> service)
{
	auto connection = QDBusConnection::sessionBus();
	if (!connection.registerService("org.freedesktop.Notifications")) {
		throw std::runtime_error{"DBus Service already registered!"};
	}
	new NotificationsAdaptor(service);
	if (!connection.registerObject("/org/freedesktop/Notifications",
				       service)) {
		throw std::runtime_error{"Can't register DBus service object!"};
	}
	return connection;
}

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);

	auto dbus_service = get_service();
	connect_to_session_bus(dbus_service);

	qmlRegisterType<IQExpirationController>("IQNotifier", 1, 0,
						"IQExpirationController");
	qmlRegisterSingletonType<IQNotifications>(
	    "IQNotifier", 1, 0, "IQNotifications", iqnotifications_provider);

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
