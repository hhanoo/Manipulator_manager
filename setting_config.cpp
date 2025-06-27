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
    settings.setValue("ROBOT_VELOCITY", static_cast<Ui::MainWindow *>(ui)->spinBox_velocity->value());
    settings.setValue("TCP_RX_DEG", static_cast<Ui::MainWindow *>(ui)->spinBox_rx->value());
    settings.setValue("TCP_RY_DEG", static_cast<Ui::MainWindow *>(ui)->spinBox_ry->value());
    settings.setValue("TCP_RZ_DEG", static_cast<Ui::MainWindow *>(ui)->spinBox_rz->value());

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
    if (!settings.value("ROBOT_VELOCITY").isNull()) {
        int velocity = settings.value("ROBOT_VELOCITY").toInt();
        static_cast<Ui::MainWindow *>(ui)->spinBox_velocity->setValue(velocity);
    }
    if (!settings.value("TCP_RX_DEG").isNull()) {
        int rx = settings.value("TCP_RX_DEG").toInt();
        static_cast<Ui::MainWindow *>(ui)->spinBox_rx->setValue(rx);
    }
    if (!settings.value("TCP_RY_DEG").isNull()) {
        int ry = settings.value("TCP_RY_DEG").toInt();
        static_cast<Ui::MainWindow *>(ui)->spinBox_ry->setValue(ry);
    }
    if (!settings.value("TCP_RZ_DEG").isNull()) {
        int rz = settings.value("TCP_RZ_DEG").toInt();
        static_cast<Ui::MainWindow *>(ui)->spinBox_rz->setValue(rz);
    }
}

QString SettingConfig::configFile() {
    QString filePath = qApp->applicationDirPath() + "/config.ini";
    return filePath;
}
