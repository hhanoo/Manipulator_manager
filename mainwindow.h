#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Eigen/Dense>
#include <QFuture>
#include <QLineEdit>
#include <QMainWindow>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
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
    int                        digital_in   = 0;
    int                        digital_out  = 0;
    std::atomic<bool>          is_moving    = false;
    QFuture<void>              move_future;
    std::array<QLineEdit *, 6> current_joint;
    std::array<QLineEdit *, 6> target_joint;
    std::array<QLineEdit *, 3> current_xyz;
    std::array<QLineEdit *, 3> target_xyz;

    // ---------------------- Main ------------------------------------
    QTimer *mainTimer;

   private slots:
    // ---------------------- Main ------------------------------------
    void statusUpdate();

    // ---------------------- Connect ------------------------------------
    void btnRobotConnect_clicked();

    // ---------------------- Joint ------------------------------------
    void btn_print_joint_clicked();
    void btn_apply_joint_clicked();
    void btn_moveJ_clicked();

    // ---------------------- TCP (X, Y, Z) ------------------------------------
    void btn_print_kinematrics_clicked();
    void btn_apply_kinematrics_clicked();
    void btn_moveL_clicked();

    // ---------------------- TCP (RX, RY, RZ) ------------------------------------
    void tcp_rotate(const char *axis, double radians, double result_T[16]);
    void btn_rx_m_clicked();
    void btn_rx_p_clicked();
    void btn_ry_m_clicked();
    void btn_ry_p_clicked();
    void btn_rz_m_clicked();
    void btn_rz_p_clicked();

    // ---------------------- Digital I/O ------------------------------------
    void checkBox_DIO_update();
};
#endif  // MAINWINDOW_H
