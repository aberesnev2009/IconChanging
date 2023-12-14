
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include "App.h"

int main(int argc, char *argv[])
{
    //	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Do not use APPLICATION_NAME here.
    // The EXECUTABLE_NAME will be used in qt standard paths. It's our goal.
    QCoreApplication::setApplicationName("");
    QApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationVersion("1.0.0");

#ifdef QT_QML_DEBUG
    QQmlDebuggingEnabler::enableDebugging(true);
#endif
    App *app = new App(argc, argv);

    QQmlApplicationEngine engine;

    int ret;
    do {
        app->initContentApp();
        ret = app->exec();
    } while (ret == App::RestartCode);
    return ret;
}
