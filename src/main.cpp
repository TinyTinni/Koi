#include "headers/mainwindow.h"
#include "headers/trayicon.h"
#include "headers/utils.h"

#include <iostream>
#include <QApplication>
#include <QLocalSocket>
#include <QLocalServer>
#include <QObject>

bool isAlreadyRunning(QString netName)
{
    QLocalSocket socket;
    socket.connectToServer(netName);
    bool isOpen = socket.isOpen();
    socket.close();
    return isOpen;
}

void createDummyNetwork(QString netName)
{
    QLocalServer *server = new QLocalServer;
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    server->listen(netName);
    return server;
}

void showConfigMenu(Utils &u)
{
    MainWindow::showMainMenu(u);
}

struct MainStructure
{
    constexpr static const char *serverName = "koiDummyNetwork";

    Utils utils;
    std::unique_ptr<TrayIcon> trayMenu;
    const std::unique_ptr<QLocalServer> dummyServer;

    MainStructure() : utils{}, trayMenu{nullptr}, dummyServer{createDummyNetwork(serverName)}
    {
        utils.initialiseSettings();
        trayMenu = std::make_unique<TrayIcon>(utils);
    }

    ~MainStructure()
    {
        MainWindow::destroyMainMenu();
    }
};

int main(int argc, char *argv[])
{
    if (isAlreadyRunning(MainStructure::serverName))
    {
        std::cout << "Another instance of Koi is already running" << std::endl;
    }
    else
    {
        QApplication a(argc, argv);

        auto *m = new MainStructure();
        if (m->utils.settings->value("start-hidden").toBool() == 0)
        {
            showConfigMenu(m->utils);
        }
        QObject::connect(m->trayMenu.get(), &TrayIcon::configRequested, m->trayMenu.get(), [m]()
                         { showConfigMenu(m->utils); });
        QObject::connect(&a, &QCoreApplication::aboutToQuit, [m]()
                         { delete m; });

        m->trayMenu->show();

        return a.exec();
    }
}
