#include "setting_config.h"

#include "ui_mainwindow.h"

SettingConfig::SettingConfig(void *_ui) {
    ui = static_cast<Ui::MainWindow *>(_ui);
}

void SettingConfig::saveConfigFile() {
    QSettings settings(configFile(), QSettings::IniFormat);

    QString robot_ip   = static_cast<Ui::MainWindow *>(ui)->txtRobotIP->text();
    QString robot_port = static_cast<Ui::MainWindow *>(ui)->txtRobotPORT->text();

    settings.setValue("ROBOT_IP", robot_ip);
    settings.setValue("ROBOT_PORT", robot_port);

    settings.sync();
}

void SettingConfig::loadConfigFile() {
    if (!QFile::exists(configFile()))
        return;

    QSettings settings(configFile(), QSettings::IniFormat);

    if (!settings.value("ROBOT_IP").isNull()) {
        QString ip = settings.value("ROBOT_IP").toString();
        static_cast<Ui::MainWindow *>(ui)->txtRobotIP->setText(ip);
    }
    if (!settings.value("ROBOT_PORT").isNull()) {
        QString port = settings.value("ROBOT_PORT").toString();
        static_cast<Ui::MainWindow *>(ui)->txtRobotPORT->setText(port);
    }
}

QString SettingConfig::configFile() {
    QString filePath = qApp->applicationDirPath() + "/config.ini";
    return filePath;
}
