#include "headers/trayicon.h"

#include <QAction>
#include <QMenu>
#include <QCoreApplication>

#include "headers/utils.h"

TrayIcon::TrayIcon(Utils& u, QObject* parent): QSystemTrayIcon{parent}, mUtils{u}
{
    setIcon(QIcon::fromTheme("koi_tray", QIcon(":/resources/icons/koi_tray.png")));
    setVisible(true);
    createMenu();
    setContextMenu(&mMenu);

    connect(this, &QSystemTrayIcon::activated, this, &TrayIcon::iconActivated);
}

void TrayIcon::createMenu()
{
    auto actionMenuQuit = new QAction("&Quit", &mMenu);
    connect(actionMenuQuit, &QAction::triggered, this, &QCoreApplication::quit, Qt::QueuedConnection);
    auto actionMenuLight = new QAction("&Light", &mMenu);
    connect(actionMenuLight, &QAction::triggered, &mUtils,
            &Utils::goLight);
    auto actionMenuDark = new QAction("&Dark", &mMenu);
    connect(actionMenuDark, &QAction::triggered, &mUtils,
            &Utils::goDark);
    auto actionMenuToggle = new QAction("&Toggle Window", &mMenu);
    connect(actionMenuToggle, &QAction::triggered, this,
            &TrayIcon::configRequested);

    mMenu.addAction(actionMenuToggle);
    mMenu.addAction(actionMenuLight);
    mMenu.addAction(actionMenuDark);
    mMenu.addAction(actionMenuQuit);
}

void TrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        Q_EMIT configRequested();
        break;

    case QSystemTrayIcon::MiddleClick:
        mUtils.toggle();
        break;
    default:
        break;
    }
}
