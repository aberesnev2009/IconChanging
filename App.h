#ifndef App_H
#define App_H

class QQmlApplicationEngine;
class QQuickWindow;

#include <QAction>
#include <QApplication>
#include <QGuiApplication>
#include <QObject>
#include <QString>
#include <QSystemTrayIcon>
#include <QtCore/QtGlobal>

/**
 * @brief The App class is the main class for working between frontend and
 * backend
 */
class App : public QApplication
{
    Q_OBJECT;

public:
  App(int &argc, char *argv[]);
  ~App();
  void initContentApp();

  static constexpr int RestartCode = 1000;
  Q_INVOKABLE void restart() { exit(RestartCode); }

  Q_INVOKABLE void quitFromApp();
  QQmlApplicationEngine *getEngine() { return mEngine; }

  /// get instance of App for other classes
  static App *getInstance() {
    return static_cast<App *>(QApplication::instance());
  }
  void openAppAfterInit(bool mustBeIconified = false);

  QQmlApplicationEngine *mEngine = nullptr;
  Q_INVOKABLE QString getAppVersion();

  /// get main window for manipulation
  QQuickWindow *getMainWindow() const;
  //////////////
  Q_INVOKABLE void setMainWindowIconState(bool start);

  Q_INVOKABLE static void smartShowWindow(QQuickWindow *window);

public slots:

signals:

  void quitSignal();

  private:
  void createConnections();

  /// register classes for work at frontend side
  void registerSharedTypes();

  /////////
  QIcon iconRun;
  QIcon iconStop;
};

#endif // App_H
