QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Client.cpp \
    LoginForm.cpp \
    MainWindow.cpp \
    Parsing.cpp \
    RegistrationForm.cpp \
    SHA_1.cpp \
    StartScreen.cpp \
    main.cpp

HEADERS += \
    Client.h \
    LoginForm.h \
    MainWindow.h \
    Parsing.h \
    RegistrationForm.h \
    SHA_1.h \
    StartScreen.h \
    sha1.hpp

FORMS += \
    LoginForm.ui \
    MainWindow.ui \
    RegistrationForm.ui \
    StartScreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
