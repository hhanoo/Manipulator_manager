#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // ---------------------- UI ------------------------------------
    ui->setupUi(this);
    this->setWindowTitle("Manipulator Operation");

    connect(ui->btnRobotConnect, &QPushButton::clicked, this, &MainWindow::btnRobotConnect_clicked);
    connect(ui->btn_print_joint, &QPushButton::clicked, this, &MainWindow::btn_print_joint_clicked);
    connect(ui->btn_print_kinematrics, &QPushButton::clicked, this, &MainWindow::btn_print_kinematrics_clicked);
    connect(ui->btn_apply_joint, &QPushButton::clicked, this, &MainWindow::btn_apply_joint_clicked);
    connect(ui->btn_apply_kinematrics, &QPushButton::clicked, this, &MainWindow::btn_apply_kinematrics_clicked);
    connect(ui->btn_moveJ, &QPushButton::clicked, this, &MainWindow::btn_moveJ_clicked);
    connect(ui->btn_moveL, &QPushButton::clicked, this, &MainWindow::btn_moveL_clicked);

    current_joint       = {ui->txt_Joint1, ui->txt_Joint2, ui->txt_Joint3, ui->txt_Joint4, ui->txt_Joint5, ui->txt_Joint6};
    current_kinematrics = {ui->txt_X, ui->txt_Y, ui->txt_Z, ui->txt_RX, ui->txt_RY, ui->txt_RZ};
    target_joint        = {ui->lineEdit_Joint1, ui->lineEdit_Joint2, ui->lineEdit_Joint3, ui->lineEdit_Joint4, ui->lineEdit_Joint5, ui->lineEdit_Joint6};
    target_kinematrics  = {ui->lineEdit_X, ui->lineEdit_Y, ui->lineEdit_Z, ui->lineEdit_RX, ui->lineEdit_RY, ui->lineEdit_RZ};

    setting_config = new SettingConfig(ui);
    setting_config->loadConfigFile();

    // ---------------------- Main -----------------------------------
    mainTimer = new QTimer(this);
    mainTimer->setInterval(50);
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(statusUpdate()));
    mainTimer->start();
}

MainWindow::~MainWindow() {
    setting_config->saveConfigFile();
    if (is_connected) {
        robot.RobotDisconnect();
    }
    delete ui;
    delete setting_config;
}

// ---------------------- Main -----------------------------------
void MainWindow::statusUpdate() {
    if (is_connected) {
        ui->btnRobotConnect->setText("Disconnect");
    } else {
        ui->btnRobotConnect->setText("Connect");
    }

    if (is_connected) {
        robot_info = robot.RobotInfo();

        for (int i = 0; i < 6; i++) {
            current_joint[i]->setText(QString::number(robot_info.jnt[i] * 180. / M_PI, 'f', 2));
        }

        current_kinematrics[0]->setText(QString::number(robot_info.mat[3] * 1000, 'f', 2));
        current_kinematrics[1]->setText(QString::number(robot_info.mat[7] * 1000, 'f', 2));
        current_kinematrics[2]->setText(QString::number(robot_info.mat[11] * 1000, 'f', 2));

        checkBox_DIO_update();

        is_connected = robot.IsConnected();
    }
}

// ---------------------- Connect ------------------------------------
void MainWindow::btnRobotConnect_clicked() {
    if (is_connected) {
        robot.RobotDisconnect();
        is_connected = false;
    } else {
        robot.SetRobotConf(robot_type, ui->txtRobotIP->text().toStdString().c_str(), ui->txtRobotPORT->text().toInt());
        robot.RobotConnect();
        robot.SetVelocity(100);
        is_connected = robot.IsConnected();
    }
}

// ---------------------- Print ------------------------------------
void MainWindow::btn_print_joint_clicked() {
    if (is_connected) {
        printf(
            "\n[Joint]: \n \
            [%.8f, %.8f, %.8f, %.8f, %.8f, %.8f]\n",
            robot_info.jnt[0], robot_info.jnt[1], robot_info.jnt[2],
            robot_info.jnt[3], robot_info.jnt[4], robot_info.jnt[5]);
    }
}

void MainWindow::btn_print_kinematrics_clicked() {
    if (is_connected) {
        printf(
            "\n[Kinematics]: \n \
            [%.5f, %.5f, %.5f, %.5f,\n \
            %.5f, %.5f, %.5f, %.5f,\n \
            %.5f, %.5f, %.5f, %.5f,\n \
            %.5f, %.5f, %.5f, %.5f]\n",
            robot_info.mat[0], robot_info.mat[1], robot_info.mat[2], robot_info.mat[3],
            robot_info.mat[4], robot_info.mat[5], robot_info.mat[6], robot_info.mat[7],
            robot_info.mat[8], robot_info.mat[9], robot_info.mat[10], robot_info.mat[11],
            robot_info.mat[12], robot_info.mat[13], robot_info.mat[14], robot_info.mat[15]);
    }
}

// ---------------------- Apply ------------------------------------
void MainWindow::btn_apply_joint_clicked() {
    if (is_connected) {
        for (int i = 0; i < 6; i++) {
            target_joint[i]->setText(current_joint[i]->text());
        }
    }
}

void MainWindow::btn_apply_kinematrics_clicked() {
    if (is_connected) {
        for (int i = 0; i < 6; i++) {
            target_kinematrics[i]->setText(current_kinematrics[i]->text());
        }
    }
}

// ---------------------- Move ------------------------------------
void MainWindow::btn_moveJ_clicked() {
    if (is_connected) {
        double target_joint_value[6];
        for (int i = 0; i < 6; i++) {
            target_joint_value[i] = target_joint[i]->text().toDouble() * M_PI / 180.;
        }
        printf("target_joint_value: [%.8f, %.8f, %.8f, %.8f, %.8f, %.8f]\n", target_joint_value[0], target_joint_value[1], target_joint_value[2], target_joint_value[3], target_joint_value[4], target_joint_value[5]);
        robot.movej(target_joint_value);
        robot.WaitMove();
    }
}

void MainWindow::btn_moveL_clicked() {
    if (is_connected) {
        double target_kinematrics_value[16];
        memcpy(target_kinematrics_value, robot_info.mat, sizeof(robot_info.mat));
        target_kinematrics_value[3]  = target_kinematrics[0]->text().toDouble() / 1000.;
        target_kinematrics_value[7]  = target_kinematrics[1]->text().toDouble() / 1000.;
        target_kinematrics_value[11] = target_kinematrics[2]->text().toDouble() / 1000.;

        printf("target_kinematrics_value: [%.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f]\n", target_kinematrics_value[0], target_kinematrics_value[1], target_kinematrics_value[2], target_kinematrics_value[3], target_kinematrics_value[4], target_kinematrics_value[5], target_kinematrics_value[6], target_kinematrics_value[7], target_kinematrics_value[8], target_kinematrics_value[9], target_kinematrics_value[10], target_kinematrics_value[11], target_kinematrics_value[12], target_kinematrics_value[13], target_kinematrics_value[14], target_kinematrics_value[15]);
        robot.movel(0, target_kinematrics_value);
        robot.WaitMove();
    }
}

// ---------------------- Digital I/O ------------------------------------
void MainWindow::checkBox_DIO_update() {
    if (is_connected) {
        //  Input
        digital_in = robot.ControlBoxDigitalIn();
        ui->checkBox_input_0->setChecked(digital_in & 0b1);
        ui->checkBox_input_1->setChecked(digital_in & 0b10);
        ui->checkBox_input_2->setChecked(digital_in & 0b100);
        ui->checkBox_input_3->setChecked(digital_in & 0b1000);
        ui->checkBox_input_4->setChecked(digital_in & 0b10000);
        ui->checkBox_input_5->setChecked(digital_in & 0b100000);
        ui->checkBox_input_6->setChecked(digital_in & 0b1000000);
        ui->checkBox_input_7->setChecked(digital_in & 0b10000000);

        //  Output
        int temp_digital_out = 0;
        if (ui->checkBox_output_0->isChecked())
            temp_digital_out += 0b1;
        if (ui->checkBox_output_1->isChecked())
            temp_digital_out += 0b10;
        if (ui->checkBox_output_2->isChecked())
            temp_digital_out += 0b100;
        if (ui->checkBox_output_3->isChecked())
            temp_digital_out += 0b1000;
        if (ui->checkBox_output_4->isChecked())
            temp_digital_out += 0b10000;
        if (ui->checkBox_output_5->isChecked())
            temp_digital_out += 0b100000;
        if (ui->checkBox_output_6->isChecked())
            temp_digital_out += 0b1000000;
        if (ui->checkBox_output_7->isChecked())
            temp_digital_out += 0b10000000;

        if (temp_digital_out != digital_out) {
            robot.ControlBoxDigitalOut(temp_digital_out);
            digital_out = temp_digital_out;
        }
    }
}