# QT 관련 ------------------------------------------------------------
# https://hub.docker.com/r/stateoftheartio/qt6
FROM stateoftheartio/qt6:6.7-gcc-aqt

# 권한 변경 ------------------------------------------------------------
USER root

# 필수 시스템 패키지 설치 ------------------------------------------------------------
RUN apt update && apt install -y \
    libgl-dev \
    libvulkan-dev \
    ninja-build \
    cmake \
    bear \
    libeigen3-dev \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

# 로케일 설정 ------------------------------------------------------------
RUN locale-gen en_US en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8
ENV LANGUAGE=en_US:en

# 워크스페이스 및 엔트리포인트 설정 ------------------------------------------------------------
WORKDIR /root/workspace
CMD ["bash"]