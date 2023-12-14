#include "App.h"

#include <QAction>
#include <QBitmap>
#include <QDebug>
#include <QDesktopServices>
#include <QMenu>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickWindow>
#include <QStyle>
#include <QTimer>
#include <qdatetime.h>

// =============================================================================

namespace {

constexpr char QmlViewMainWindow[] = "qrc:/main.qml";
constexpr char MainQmlUri[] = "Test";
} // namespace

App::App(int &argc, char *argv[])
    : QApplication(argc, argv)
{
    this->setQuitOnLastWindowClosed(false);

    QSurfaceFormat format;
    format.setSamples(8);
    QSurfaceFormat::setDefaultFormat(format);
}

App::~App()
{
    qInfo() << QStringLiteral("Destroying app...");
    delete mEngine;
}

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    QString logType;

    //  QRegExp rx("([\\w-]+::[\\w-]+)");
    //  if (rx.indexIn(context.function) == -1)
    //    return;
    //  QString function = rx.cap(1);
    QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    txt += QString("[%1] ").arg(dt);

    switch (type) {
    case QtInfoMsg:
        txt += QString("Info: %1").arg(msg);
        logType = "Info";
        break;
    case QtDebugMsg:
        txt += QString("Debug: %1").arg(msg);
        logType = "Debug";
        break;
    case QtWarningMsg:
        txt += QString("Warning: %1").arg(msg);
        logType = "Warning";
        break;
    case QtCriticalMsg:
        txt += QString("Critical: %1").arg(msg);
        logType = "Critical";
        break;
    case QtFatalMsg:
        txt += QString("Fatal: %1").arg(msg);
        logType = "Fatal";
        abort();
        break;
    default:
        txt += QString("unknown: %1").arg(msg);
        logType = "unknown";
    }
    // std::c << logType.toStdString() << ":" << txt.toStdString() << +"\n";
    //App::getInstance()->getFileWorker()->saveLog(txt, logType);
    qInfo().noquote() << msg;
}

QQuickWindow *App::getMainWindow() const
{
    return qobject_cast<QQuickWindow *>(
        const_cast<QQmlApplicationEngine *>(mEngine)->rootObjects().at(0));
}

#include "QPixmap"
void App::setMainWindowIconState(bool start)
{
    QQuickWindow *mainWindow = getMainWindow();
    QIcon icon;
    if (start) {
        icon = iconRun;
    } else {
        icon = iconStop;
    }
    mainWindow->setIcon(icon);
    //App::getInstance()->setWindowIcon(icon);
}

void App::smartShowWindow(QQuickWindow *window)
{
    if (!window)
        return;
    window->setVisible(true);
    // Force show, maybe redundant with setVisible
    if (window->visibility() == QWindow::Maximized) // Avoid to change visibility mode
        window->showMaximized();
    else
        window->show();

    window->raise(); // Raise ensure to get focus on Mac
    window->requestActivate();
}

void App::createConnections() {}

static QQuickWindow *createSubWindow(QQmlApplicationEngine *engine, QString path)
{
    qInfo() << QStringLiteral("creating subwindow: '%1'").arg(path);

    QQmlComponent component(engine, QUrl(path));
    if (component.isError()) {
        qWarning() << component.errors();
        abort();
    }

    qInfo() << QStringLiteral("subwindow status: '%1'").arg(component.status());

    QObject *object = component.create();
    Q_ASSERT(object);

    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
    object->setParent(engine);

    return qobject_cast<QQuickWindow *>(object);
}

template<typename T, T *(*function)()>
static QObject *makeSharedSingleton(QQmlEngine *, QJSEngine *)
{
    QObject *object = (*function)();
    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
    return object;
}

template<typename T, T *(*function)(void)>
static inline void registerSharedSingletonType(const char *name)
{
    qmlRegisterSingletonType<T>(MainQmlUri, 1, 0, name, makeSharedSingleton<T, function>);
}

template<typename T, T *(App::*function)() const>
static QObject *makeSharedSingleton(QQmlEngine *, QJSEngine *)
{
    QObject *object = (App::getInstance()->*function)();
    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
    return object;
}

template<typename T, T *(App::*function)() const>
static inline void registerSharedSingletonType(const char *name)
{
    qmlRegisterSingletonType<T>(MainQmlUri, 1, 0, name, makeSharedSingleton<T, function>);
}

template<typename T>
static inline void registerType(const char *name)
{
    qmlRegisterType<T>(MainQmlUri, 1, 0, name);
}

void App::initContentApp()
{
    if (mEngine) {
        qInfo() << QStringLiteral("Restarting app...");
        delete mEngine;

    } else {
    }

    // Init engine content.
    mEngine = new QQmlApplicationEngine();

    qInstallMessageHandler(logMessageHandler);
    qInfo().noquote() << "           "
                         "#######################################################"
                         "###           ";
    qInfo().noquote() << QStringLiteral("Init engine. App version:") << getAppVersion();
    iconRun = QIcon(":/Resources/pause_red.svg");
    iconStop = QIcon(":/Resources/play_green.svg");

    registerSharedTypes();

    createConnections();
    openAppAfterInit();
}


void App::quitFromApp()
{

}

void App::openAppAfterInit(bool mustBeIconified)
{
    qInfo().noquote() << QStringLiteral("Loading main view...");

    mEngine->load(QUrl(QmlViewMainWindow));

    if (mEngine->rootObjects().isEmpty())
        qFatal("Unable to open main window.");

    QQuickWindow *mainWindow = getMainWindow();

    setMainWindowIconState(false);

    // Show main window.
    Q_UNUSED(mustBeIconified);
    smartShowWindow(mainWindow);
}

QString App::getAppVersion()
{
    return QCoreApplication::applicationVersion();
}

void App::registerSharedTypes()
{
    qInfo().noquote() << QStringLiteral("Registering shared types...");

    registerSharedSingletonType<App, &App::getInstance>("App");
}
