#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // ---------------------- UI ------------------------------------
    ui->setupUi(this);
    this->setWindowTitle("Manipulator Manager");

    // Connect
    connect(ui->btnRobotConnect, &QPushButton::clicked, this, &MainWindow::btnRobotConnect_clicked);

    // Joint
    connect(ui->btn_print_joint, &QPushButton::clicked, this, &MainWindow::btn_print_joint_clicked);
    connect(ui->btn_apply_joint, &QPushButton::clicked, this, &MainWindow::btn_apply_joint_clicked);
    connect(ui->btn_moveJ, &QPushButton::clicked, this, &MainWindow::btn_moveJ_clicked);
    current_joint = {ui->txt_Joint1, ui->txt_Joint2, ui->txt_Joint3, ui->txt_Joint4, ui->txt_Joint5, ui->txt_Joint6};
    target_joint  = {ui->lineEdit_Joint1, ui->lineEdit_Joint2, ui->lineEdit_Joint3, ui->lineEdit_Joint4, ui->lineEdit_Joint5, ui->lineEdit_Joint6};

    // TCP (X, Y, Z)
    connect(ui->btn_print_kinematrics, &QPushButton::clicked, this, &MainWindow::btn_print_kinematrics_clicked);
    connect(ui->btn_apply_kinematrics, &QPushButton::clicked, this, &MainWindow::btn_apply_kinematrics_clicked);
    connect(ui->btn_moveL, &QPushButton::clicked, this, &MainWindow::btn_moveL_clicked);
    current_xyz = {ui->txt_X, ui->txt_Y, ui->txt_Z};
    target_xyz  = {ui->lineEdit_X, ui->lineEdit_Y, ui->lineEdit_Z};

    // TCP (RX, RY, RZ)
    connect(ui->btn_rx_m, &QPushButton::clicked, this, &MainWindow::btn_rx_m_clicked);
    connect(ui->btn_rx_p, &QPushButton::clicked, this, &MainWindow::btn_rx_p_clicked);
    connect(ui->btn_ry_m, &QPushButton::clicked, this, &MainWindow::btn_ry_m_clicked);
    connect(ui->btn_ry_p, &QPushButton::clicked, this, &MainWindow::btn_ry_p_clicked);
    connect(ui->btn_rz_m, &QPushButton::clicked, this, &MainWindow::btn_rz_m_clicked);
    connect(ui->btn_rz_p, &QPushButton::clicked, this, &MainWindow::btn_rz_p_clicked);

    // UI Setting
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
    // Set button enable/disable
    if (is_connected && !is_moving) {
        ui->btn_moveJ->setEnabled(true);
        ui->btn_moveL->setEnabled(true);
        ui->btn_rx_m->setEnabled(true);
        ui->btn_rx_p->setEnabled(true);
        ui->btn_ry_m->setEnabled(true);
        ui->btn_ry_p->setEnabled(true);
        ui->btn_rz_m->setEnabled(true);
        ui->btn_rz_p->setEnabled(true);
    } else {
        ui->btn_moveJ->setEnabled(false);
        ui->btn_moveL->setEnabled(false);
        ui->btn_rx_m->setEnabled(false);
        ui->btn_rx_p->setEnabled(false);
        ui->btn_ry_m->setEnabled(false);
        ui->btn_ry_p->setEnabled(false);
        ui->btn_rz_m->setEnabled(false);
        ui->btn_rz_p->setEnabled(false);
    }

    // Set button text
    if (is_connected) {
        ui->btnRobotConnect->setText("Disconnect");
    } else {
        ui->btnRobotConnect->setText("Connect");
    }

    // Set robot info
    if (is_connected) {
        robot_info = robot.RobotInfo();

        for (int i = 0; i < 6; i++) {
            current_joint[i]->setText(QString::number(robot_info.jnt[i] * 180. / M_PI, 'f', 2));
        }

        current_xyz[0]->setText(QString::number(robot_info.mat[3] * 1000, 'f', 2));
        current_xyz[1]->setText(QString::number(robot_info.mat[7] * 1000, 'f', 2));
        current_xyz[2]->setText(QString::number(robot_info.mat[11] * 1000, 'f', 2));

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

// ---------------------- Joint ------------------------------------
void MainWindow::btn_print_joint_clicked() {
    if (is_connected) {
        printf(
            "\n[Joint]: \n \
            [%.8f, %.8f, %.8f, %.8f, %.8f, %.8f]\n",
            robot_info.jnt[0], robot_info.jnt[1], robot_info.jnt[2],
            robot_info.jnt[3], robot_info.jnt[4], robot_info.jnt[5]);
    }
}

void MainWindow::btn_apply_joint_clicked() {
    if (is_connected) {
        for (int i = 0; i < 6; i++) {
            target_joint[i]->setText(current_joint[i]->text());
        }
    }
}
void MainWindow::btn_moveJ_clicked() {
    if (is_connected && !is_moving) {
        is_moving.store(true);

        std::array<double, 6> target_J;
        for (int i = 0; i < 6; i++) {
            target_J[i] = target_joint[i]->text().toDouble() * M_PI / 180.;
        }

        move_future = QtConcurrent::run([this, target_J]() {
            robot.SetVelocity(ui->spinBox_velocity->value());
            robot.movej(const_cast<double *>(target_J.data()));
            robot.WaitMove();
            is_moving.store(false);
        });
    }
}

// ---------------------- TCP (X, Y, Z) ------------------------------------
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

void MainWindow::btn_apply_kinematrics_clicked() {
    if (is_connected) {
        for (int i = 0; i < 3; i++) {
            target_xyz[i]->setText(current_xyz[i]->text());
        }
    }
}

void MainWindow::btn_moveL_clicked() {
    if (is_connected && !is_moving) {
        is_moving.store(true);

        double target_T[16];
        memcpy(target_T, robot_info.mat, sizeof(robot_info.mat));
        target_T[3]  = target_xyz[0]->text().toDouble() / 1000.;
        target_T[7]  = target_xyz[1]->text().toDouble() / 1000.;
        target_T[11] = target_xyz[2]->text().toDouble() / 1000.;

        std::array<double, 16> move_T;
        std::copy(begin(target_T), end(target_T), move_T.begin());

        move_future = QtConcurrent::run([this, move_T]() {
            robot.SetVelocity(ui->spinBox_velocity->value());
            robot.movel(0, const_cast<double *>(move_T.data()));
            robot.WaitMove();
            is_moving.store(false);
        });
    }
}

// ---------------------- TCP (RX, RY, RZ) ------------------------------------
void MainWindow::tcp_rotate(const char *axis, double radians, double result_T[16]) {
    double current_T[16];
    memcpy(current_T, robot_info.mat, sizeof(robot_info.mat));

    // Convert 1D array to Eigen 4x4 row-major matrix
    Eigen::Map<const Eigen::Matrix<double, 4, 4, Eigen::RowMajor>> current_T_matrix(current_T);

    // Convert 4x4 matrix to affine transformation
    Eigen::Transform<double, 3, Eigen::Affine, Eigen::RowMajor> current_affine(current_T_matrix);

    // Create rotation affine transformation
    Eigen::Transform<double, 3, Eigen::Affine, Eigen::RowMajor> rotation_affine = Eigen::Transform<double, 3, Eigen::Affine, Eigen::RowMajor>::Identity();

    switch (*axis) {
        case 'X':
            rotation_affine.rotate(Eigen::AngleAxisd(radians, Eigen::Vector3d::UnitX()));
            break;
        case 'Y':
            rotation_affine.rotate(Eigen::AngleAxisd(radians, Eigen::Vector3d::UnitY()));
            break;
        case 'Z':
            rotation_affine.rotate(Eigen::AngleAxisd(radians, Eigen::Vector3d::UnitZ()));
            break;
        default:
            std::cerr << "Invalid axis: " << axis << std::endl;
            break;
    }

    // Apply rotation to target affine transformation
    Eigen::Transform<double, 3, Eigen::Affine, Eigen::RowMajor> result_affine   = current_affine * rotation_affine;
    Eigen::Matrix<double, 4, 4, Eigen::RowMajor>                result_T_matrix = result_affine.matrix();

    // Map output_1D as a 4x4 row-major matrix
    Eigen::Map<Eigen::Matrix<double, 4, 4, Eigen::RowMajor>> result_T_matrix_map(result_T);
    result_T_matrix_map = result_T_matrix;
}

void MainWindow::btn_rx_m_clicked() {
    if (is_connected && !is_moving) {
        is_moving.store(true);

        double radians = ui->spinBox_rx->value() * M_PI / 180.0;
        double result_T[16];

        tcp_rotate("X", -radians, result_T);

        std::array<double, 16> move_T;
        std::copy(begin(result_T), end(result_T), move_T.begin());

        move_future = QtConcurrent::run([this, move_T]() {
            robot.SetVelocity(ui->spinBox_velocity->value());
            robot.movel(0, const_cast<double *>(move_T.data()));
            robot.WaitMove();
            is_moving.store(false);
        });
    }
}

void MainWindow::btn_rx_p_clicked() {
    if (is_connected && !is_moving) {
        is_moving.store(true);

        double radians = ui->spinBox_rx->value() * M_PI / 180.0;
        double result_T[16];

        tcp_rotate("X", radians, result_T);

        std::array<double, 16> move_T;
        std::copy(begin(result_T), end(result_T), move_T.begin());

        move_future = QtConcurrent::run([this, move_T]() {
            robot.SetVelocity(ui->spinBox_velocity->value());
            robot.movel(0, const_cast<double *>(move_T.data()));
            robot.WaitMove();
            is_moving.store(false);
        });
    }
}

void MainWindow::btn_ry_m_clicked() {
    if (is_connected && !is_moving) {
        is_moving.store(true);

        double radians = ui->spinBox_ry->value() * M_PI / 180.0;
        double result_T[16];

        tcp_rotate("Y", -radians, result_T);

        std::array<double, 16> move_T;
        std::copy(begin(result_T), end(result_T), move_T.begin());

        move_future = QtConcurrent::run([this, move_T]() {
            robot.SetVelocity(ui->spinBox_velocity->value());
            robot.movel(0, const_cast<double *>(move_T.data()));
            robot.WaitMove();
            is_moving.store(false);
        });
    }
}

void MainWindow::btn_ry_p_clicked() {
    if (is_connected && !is_moving) {
        is_moving.store(true);

        double radians = ui->spinBox_ry->value() * M_PI / 180.0;
        double result_T[16];

        tcp_rotate("Y", radians, result_T);

        std::array<double, 16> move_T;
        std::copy(begin(result_T), end(result_T), move_T.begin());

        move_future = QtConcurrent::run([this, move_T]() {
            robot.SetVelocity(ui->spinBox_velocity->value());
            robot.movel(0, const_cast<double *>(move_T.data()));
            robot.WaitMove();
            is_moving.store(false);
        });
    }
}

void MainWindow::btn_rz_m_clicked() {
    if (is_connected && !is_moving) {
        is_moving.store(true);

        double radians = ui->spinBox_rz->value() * M_PI / 180.0;
        double result_T[16];

        tcp_rotate("Z", -radians, result_T);

        std::array<double, 16> move_T;
        std::copy(begin(result_T), end(result_T), move_T.begin());

        move_future = QtConcurrent::run([this, move_T]() {
            robot.SetVelocity(ui->spinBox_velocity->value());
            robot.movel(0, const_cast<double *>(move_T.data()));
            robot.WaitMove();
            is_moving.store(false);
        });
    }
}

void MainWindow::btn_rz_p_clicked() {
    if (is_connected && !is_moving) {
        is_moving.store(true);

        double radians = ui->spinBox_rz->value() * M_PI / 180.0;
        double result_T[16];

        tcp_rotate("Z", radians, result_T);

        std::array<double, 16> move_T;
        std::copy(begin(result_T), end(result_T), move_T.begin());

        move_future = QtConcurrent::run([this, move_T]() {
            robot.SetVelocity(ui->spinBox_velocity->value());
            robot.movel(0, const_cast<double *>(move_T.data()));
            robot.WaitMove();
            is_moving.store(false);
        });
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