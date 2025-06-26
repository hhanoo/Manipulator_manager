QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    setting_config.cpp

HEADERS += \
    mainwindow.h
    setting_config.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Include paths for header files
INCLUDEPATH += $$PWD/KETIRobotSDK
INCLUDEPATH += /usr/include/eigen3

# Specify dependency paths
DEPENDPATH += $$PWD/KETIRobotSDK

# Link external libraries
LIBS += -L$$PWD/KETIRobotSDK -lrobotsdk

# Add library path to runtime library search path
QMAKE_RPATHDIR += $$PWD/KETIRobotSDK

# Compiler flags
QMAKE_CXXFLAGS += -DUA_ARCHITECTURE_POSIX