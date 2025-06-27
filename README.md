# Manipulator Operation

## 📁 Project Structure

```
Manipulator_operation/
├── build/                          # Build output directory
│   └── debug/                       Debug build
├── KETIRobotSDK/                   # Robot SDK directory
│   ├── ...
│   └── sdkv2.h                    # Robot SDK header
├── main.cpp                        # Program entry point
├── mainwindow.h / .cpp             # Main window implementation
├── mainwindow.ui                   # Qt Designer UI file
├── setting_config.h / .cpp         # Configuration management
├── Manipulator_operation.pro       # Qt project file
├── Dockerfile                      # Docker build configuration
└── README.md                       # Project documentation
```

## 🔧 Environment & Installation

### Docker Environment Setup

<details>
<summary>Installation Guide</summary>

#### 1. Docker Image

```bash
docker build -t manipulator_operation -f Dockerfile .
```

#### 2. Run Docker Container

```bash
# Allow GUI sharing (run once)
xhost +local:
```

```bash
# Run container
docker run -it \
    --privileged \
    --network host \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -e LIBGL_ALWAYS_INDIRECT=1 \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v /dev:/dev \
    -v ${USER PROJECT PATH}/Manipulator_operation:/root/workspace/ \
    --name Manipulator_operation \

    manipulator_operation
```

#### 3. Compile in container

```bash
cd /root/workspace
mkdir -p build
```

```bash
cd /root/workspace/build && /opt/Qt/6.7.0/gcc_64/bin/qmake ../Manipulator_operation.pro && bear make
```

#### 4. Kpick Installation

```bash
/root/workspace/build/Manipulator_operation
```

</details>

## 🚀 Usage

### 1. Starting the System

```bash
/root/workspace/build/Manipulator_operation
```

### 2. Main Features

- **Robot Connection/Disconnection**: TCP/IP-based robot communication
- **Joint Control**: Real-time monitoring and control of 6-axis joint values
- **Kinematics Control**: Position/orientation-based linear/joint movement
- **Digital I/O**: 8-channel digital input/output control
- **Real-time Monitoring**: 50ms cycle status updates

## ⚙️ Configuration

### 1. System Architecture

- TCP/IP socket-based architecture
- Robot SDK: KETIRobotSDK
- Qt 6.7.x GUI framework

## 📝 Notes

- Check robot connections before starting
- Verify network connectivity between systems
