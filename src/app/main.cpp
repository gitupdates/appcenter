/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtCore/QStandardPaths>
#include <QDebug>
#include <QIcon>
#include <QDir>
#include <QTranslator>
#include <QtQuickControls2/QQuickStyle>

static void loadQtTranslations()
{
#ifndef QT_NO_TRANSLATION
    QString locale = QLocale::system().name();

    // Load Qt translations
    QTranslator *qtTranslator = new QTranslator(QCoreApplication::instance());
    if (qtTranslator->load(QStringLiteral("qt_%1").arg(locale), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        qApp->installTranslator(qtTranslator);
    } else {
        delete qtTranslator;
    }
#endif
}

static void loadAppTranslations()
{
#ifndef QT_NO_TRANSLATION
    QString locale = QLocale::system().name();

    // Find the translations directory
    const QString path = QLatin1String("liri-appcenter/translations");
    const QString translationsDir =
        QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                               path,
                               QStandardPaths::LocateDirectory);

    // Load shell translations
    QTranslator *appTranslator = new QTranslator(QCoreApplication::instance());
    if (appTranslator->load(QStringLiteral("%1/appcenter_%3").arg(translationsDir, locale))) {
        QCoreApplication::installTranslator(appTranslator);
    } else if (locale == QLatin1String("C") ||
                locale.startsWith(QLatin1String("en"))) {
        // English is the default, it's translated anyway
        delete appTranslator;
    }
#endif
}

int main(int argc, char *argv[])
{
    // Set the X11 WML_CLASS so X11 desktops can find the desktop file
    qputenv("RESOURCE_NAME", QByteArrayLiteral("io.liri.AppCenter"));

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationName(QLatin1String("App Center"));
    app.setOrganizationDomain(QLatin1String("liri.io"));
    app.setOrganizationName(QLatin1String("Liri"));
    app.setDesktopFileName(QLatin1String("io.liri.AppCenter.desktop"));

    QQuickStyle::setStyle(QLatin1String("Material"));

    // Load translations
    loadQtTranslations();
    loadAppTranslations();

    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
