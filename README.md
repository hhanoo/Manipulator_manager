# Manipulator Manager

**Qt 기반 6축 매니퓰레이터 실시간 제어 및 모니터링 GUI 시스템**

[![C++](https://img.shields.io/badge/C++-17-blue)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-6.7-green)](https://www.qt.io/)
[![License](https://img.shields.io/badge/License-MIT-orange)](LICENSE)
[![Docker](https://img.shields.io/badge/Docker-Supported-brightgreen)](docker/)

## 목차

- [데모](#데모)
- [개요](#개요)
- [주요 기능](#주요-기능)
- [빠른 시작](#빠른-시작)
- [시스템 요구사항](#시스템-요구사항)
- [설치](#설치)
- [실행](#실행)
- [사용법](#사용법)
- [설정](#설정)
- [문제 해결](#문제-해결)
- [라이선스](#라이선스)

---

## 데모

### 시스템 구조

```
┌──────────────────────────────────────────────────────────┐
│                  Manipulator Manager System               │
└──────────────────────────────────────────────────────────┘

    6-Axis Manipulator (UR10 등)
              │
              │ TCP/IP
              ▼
    ┌──────────────────────┐
    │ KETIRobotSDK         │ (C++)
    │ - Robot communication│
    │ - Joint/Pose control │
    │ - Digital I/O        │
    └──────────┬───────────┘
               │ SDK API
               ▼
    ┌──────────────────────┐
    │ Manipulator Manager  │ (C++ / Qt 6)
    │ - GUI Interface      │
    │ - Real-time monitor  │
    │ - Joint/Kinematics   │
    │ - Digital I/O panel  │
    └──────────────────────┘
```

### 프로젝트 구조

```
Manipulator_manager/
├── KETIRobotSDK/                  # Robot SDK 라이브러리
│   ├── librobotsdk.so             # 공유 라이브러리
│   ├── sdkv2.h                    # SDK 헤더
│   └── robotconf.h                # 로봇 설정 헤더
├── docker/                        # Docker 지원
│   ├── Dockerfile                 # Docker 이미지 빌드 설정
│   ├── build.sh                   # 이미지 빌드 스크립트
│   ├── run.sh                     # 컨테이너 실행 스크립트
│   └── config.sh.example          # 설정 템플릿
├── docs/                          # 문서
│   └── preview.png                # GUI 스크린샷
├── main.cpp                       # 프로그램 진입점
├── mainwindow.h / .cpp            # 메인 윈도우 구현
├── mainwindow.ui                  # Qt Designer UI 파일
├── setting_config.h / .cpp        # 설정 관리
├── Manipulator_manager.pro        # Qt 프로젝트 파일
└── README.md                      # 프로젝트 문서
```

### 스크린샷

![GUI](docs/preview.png)

---

## 개요

### 프로젝트 목적

Manipulator Manager는 KETI Robot SDK를 활용하여 6축 산업용 매니퓰레이터를 실시간으로 제어하고 모니터링하는 Qt 기반 GUI 애플리케이션입니다. TCP/IP 통신을 통해 로봇과 연결하며, 관절 제어, 기구학 제어, 디지털 I/O 제어 등 로봇 운용에 필요한 핵심 기능을 제공합니다.

### 주요 구성요소

- **KETIRobotSDK** (C++): KETI 로봇 통신 SDK ([GitHub](https://github.com/robot-plus-program/ketirobotsdk))
- **MainWindow** (C++ / Qt): 로봇 제어 및 모니터링 GUI
- **SettingConfig** (C++ / Qt): 연결 설정 및 구성 파일 관리

### 적용 가능 영역

- 산업용 매니퓰레이터 수동 제어 및 티칭
- 로봇 관절값 및 TCP 포즈 실시간 모니터링
- 디지털 I/O 기반 주변 장치 제어
- 로봇 SDK 테스트 및 디버깅
- 연구 개발 및 교육

---

## 주요 기능

- **로봇 연결/해제** - TCP/IP 기반 로봇 통신 연결 및 상태 관리
- **관절 제어** - 6축 관절값 실시간 모니터링 및 MoveJ 제어
- **기구학 제어** - X/Y/Z 위치 기반 MoveL (직선 이동) 제어
- **TCP 회전 제어** - RX, RY, RZ 축 +/- 버튼을 통한 TCP 회전
- **디지털 I/O** - 8채널 디지털 입출력 모니터링 및 제어
- **실시간 모니터링** - 50ms 주기 상태 업데이트
- **속도 제어** - 로봇 이동 속도 조절 (1-100%)
- **설정 저장/로드** - config.ini를 통한 연결 정보 자동 저장

---

## 빠른 시작

### Option 1: Docker (권장)

```bash
# 1. 저장소 클론
git clone <repository-url> ~/Manipulator_manager
cd ~/Manipulator_manager

# 2. Docker 이미지 빌드
cd docker
./build.sh

# 3. Docker 컨테이너 실행
./run.sh

# 4. 컨테이너 내에서 빌드 및 실행
cd /root/workspace/Manipulator_manager
mkdir -p build && cd build
/opt/Qt/6.7.0/gcc_64/bin/qmake ../Manipulator_manager.pro
make
./Manipulator_manager
```

### Option 2: Native Installation

```bash
# 1. Qt 6.7 설치 후 프로젝트 빌드
cd ~/Manipulator_manager
mkdir -p build && cd build
qmake ../Manipulator_manager.pro
make

# 2. 실행
./Manipulator_manager
```

---

## 시스템 요구사항

### 필수

- **OS**: Ubuntu 22.04 LTS
- **C++**: C++17 compiler (GCC 9+)
- **Qt**: 6.7.x
- **CMake/qmake**: Qt 6 호환 빌드 도구

### 하드웨어

- **Network**: 로봇과 TCP/IP 통신 가능한 네트워크 환경
- **Display**: GUI 표시를 위한 X11 디스플레이 (Docker 사용 시 X11 포워딩 필요)

### 소프트웨어 의존성

- **Eigen3** - 선형대수 라이브러리 (`libeigen3-dev`)
- **libcurl** - HTTP 통신 (`libcurl4-openssl-dev`)
- **KETIRobotSDK** - KETI 로봇 SDK (프로젝트에 포함)
- **Docker** - 컨테이너 환경 (선택)

---

## 설치

### Method 1: Docker (권장)

Docker를 사용하면 모든 의존성이 자동으로 설치됩니다:

```bash
cd ~/Manipulator_manager/docker
./build.sh
```

빌드 완료 후 `./run.sh`로 컨테이너를 실행하면 모든 환경이 준비됩니다.

### Method 2: Native Installation

#### 1. 시스템 의존성 설치

```bash
sudo apt update
sudo apt install -y \
  libgl-dev \
  libvulkan-dev \
  ninja-build \
  cmake \
  bear \
  libeigen3-dev \
  libcurl4-openssl-dev
```

#### 2. Qt 6.7 설치

Qt 공식 설치 도구를 사용하거나 `aqt`로 설치:

- [Qt Online Installer](https://www.qt.io/download)

#### 3. 저장소 클론

```bash
git clone <repository-url> ~/Manipulator_manager
```

---

## 실행

### Docker 실행

```bash
# Docker 컨테이너 실행
cd ~/Manipulator_manager/docker
./run.sh

# 컨테이너 내부에서 빌드
mkdir -p build && cd build
/opt/Qt/6.7.0/gcc_64/bin/qmake ../Manipulator_manager.pro
bear make

# 애플리케이션 실행
./Manipulator_manager
```

### Native 실행

```bash
# Qt Creator에서 열기
qtcreator Manipulator_manager.pro

# 또는 CLI로 빌드 및 실행
mkdir -p build && cd build
qmake ../Manipulator_manager.pro
make
./Manipulator_manager
```

---

## 사용법

### Basic Workflow

1. **로봇 연결**

   ```
   GUI에서 로봇 IP 주소 입력 후 Connect 버튼 클릭
   연결 성공 시 상태 표시등 변경
   ```

2. **관절값 확인**

   ```
   Current Joint 영역에서 6축 관절값 실시간 확인
   Print 버튼으로 현재 관절값 출력
   ```

3. **관절 이동 (MoveJ)**

   ```
   Target Joint 영역에 목표 관절값 입력
   Apply 버튼으로 값 적용 후 MoveJ 버튼 클릭
   ```

4. **직선 이동 (MoveL)**

   ```
   Target XYZ 영역에 목표 위치 입력
   Apply 버튼으로 값 적용 후 MoveL 버튼 클릭
   ```

5. **TCP 회전**

   ```
   RX/RY/RZ 축별 +/- 버튼으로 TCP 회전 제어
   ```

6. **디지털 I/O 제어**

   ```
   체크박스를 통해 8채널 디지털 출력 On/Off 제어
   디지털 입력 상태 실시간 모니터링
   ```

---

## 설정

### Docker 설정

[docker/config.sh.example](docker/config.sh.example)을 편집하여 Docker 환경을 설정합니다.
첫 실행 시 `config.sh.example`에서 `config.sh`로 자동 복사됩니다.

| 파라미터         | 기본값                | 설명                 |
| ---------------- | --------------------- | -------------------- |
| `IMAGE_NAME`     | `manipulator-manager` | Docker 이미지 이름   |
| `CONTAINER_NAME` | `manipulator-manager` | Docker 컨테이너 이름 |

### 앱 설정

애플리케이션 설정은 `config.ini` 파일에 자동 저장/로드됩니다.

| 파라미터   | 설명                    |
| ---------- | ----------------------- |
| Robot IP   | 로봇 TCP/IP 연결 주소   |
| Robot Port | 로봇 TCP/IP 연결 포트   |
| Velocity   | 로봇 이동 속도 (1-100%) |

---

## 문제 해결

### 1. Docker / GUI 관련

#### 1-1. Problem: Docker 컨테이너에서 GUI가 표시되지 않음

**Solution:**

```bash
# X11 포워딩 활성화 확인
xhost +local:docker

# DISPLAY 환경변수 확인
echo $DISPLAY

# run.sh가 X11 설정을 자동으로 처리하므로 ./run.sh 사용 권장
```

#### 1-2. Problem: "cannot open display" 에러

**Solution:**

- SSH 접속 시 `-X` 옵션으로 X11 포워딩 활성화
- 로컬 환경에서 `$DISPLAY` 환경변수가 설정되어 있는지 확인

### 2. 빌드 관련

#### 2-1. Problem: "libeigen3-dev not found"

**Solution:**

```bash
sudo apt update
sudo apt install -y libeigen3-dev
```

#### 2-2. Problem: Qt 라이브러리를 찾지 못함

**Solution:**

```bash
# Qt 설치 경로 확인
ls /opt/Qt/

# Docker 내에서는 아래 경로 사용
/opt/Qt/6.7.0/gcc_64/bin/qmake ../Manipulator_manager.pro
```

### 3. 로봇 연결 관련

#### 3-1. Problem: 로봇에 연결되지 않음

**Solution:**

1. 로봇 전원이 켜져 있고 네트워크에 접근 가능한지 확인
2. 로봇 IP 주소와 포트가 올바른지 확인
3. 방화벽 설정 확인
4. `ping <robot-ip>`로 네트워크 연결 테스트

#### 3-2. Problem: Docker 컨테이너에서 로봇 통신 불가

**Solution:**

- `run.sh`는 `--network host` 옵션을 사용하므로 호스트와 동일한 네트워크 접근 가능
- 호스트에서 로봇 통신이 가능한지 먼저 확인

---

## 라이선스

TBD

---

**Maintainer**: hhanoo (woo980711@gmail.com)
