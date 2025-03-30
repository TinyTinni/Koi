#pragma once

#include <QSystemTrayIcon>
#include <QMenu>

class Utils;

class TrayIcon : public QSystemTrayIcon
{
  Q_OBJECT
public:
  TrayIcon(Utils& utils, QObject* parent = nullptr);

Q_SIGNALS:
  void configRequested();

private:
  void iconActivated(QSystemTrayIcon::ActivationReason reason);

  Utils& mUtils;

  void createMenu();
  QMenu mMenu;
};
