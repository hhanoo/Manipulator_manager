#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Eigen/Dense>
#include <QLineEdit>
#include <QMainWindow>
#include <QTimer>
#include <array>
#include <cmath>

#include "sdkv2.h"
#include "setting_config.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private:
    // ---------------------- UI -------------------------------------
    Ui::MainWindow *ui;
    SettingConfig  *setting_config;

    double target_T[16];

    // ---------------------- Robot ------------------------------------
    sdk      robot;
    sdk_info robot_info;
    int      robot_type = UR10;

    bool                       is_connected = false;
    std::array<QLineEdit *, 6> current_joint;
    std::array<QLineEdit *, 6> current_kinematrics;
    std::array<QLineEdit *, 6> target_joint;
    std::array<QLineEdit *, 6> target_kinematrics;
    int                        digital_in  = 0;
    int                        digital_out = 0;

    // ---------------------- Main ------------------------------------
    QTimer *mainTimer;

   private slots:
    // ---------------------- Main ------------------------------------
    void statusUpdate();

    // ---------------------- Connect ------------------------------------
    void btnRobotConnect_clicked();

    // ---------------------- Print ------------------------------------
    void btn_print_joint_clicked();
    void btn_print_kinematrics_clicked();

    // ---------------------- Apply ------------------------------------
    void btn_apply_joint_clicked();
    void btn_apply_kinematrics_clicked();

    // ---------------------- Move ------------------------------------
    void btn_moveJ_clicked();
    void btn_moveL_clicked();

    // ---------------------- Digital I/O ------------------------------------
    void checkBox_DIO_update();
};
#endif  // MAINWINDOW_H
