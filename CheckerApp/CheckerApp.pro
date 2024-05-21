QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH +=D:\Program Files\WpdPack_4_1_2\WpdPack\Include
LIBS += "-LD:\Program Files\WpdPack_4_1_2\WpdPack\Lib"
LIBS += -lws2_32

QT += widgets

SOURCES += \
    Logger.cpp \
    admin.cpp \
    clientconnection.cpp \
    codeeditor.cpp \
    homewindowb.cpp \
    homeworks.cpp \
    homeworksolution.cpp \
    iuser.cpp \
    main.cpp \
    mainwindow.cpp \
    professor.cpp \
    signup.cpp \
    statistics.cpp \
    student.cpp \
    user.cpp \
    utils.cpp

HEADERS += \
    Logger.h \
    admin.h \
    clientconnection.h \
    codeeditor.h \
    homewindowb.h \
    homeworks.h \
    homeworksolution.h \
    iuser.h \
    mainwindow.h \
    professor.h \
    signup.h \
    statistics.h \
    student.h \
    user.h \
    utils.h

FORMS += \
    clientconnection.ui \
    homewindowb.ui \
    mainwindow.ui \
    signup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    image/book-stack-xxl.png \
    image/check-mark-xxl.png \
    image/compilate.jpg \
    image/image.png \
    image/imageSignUp.jpg \
    image/image_user.png \
    image/log_out.png \
    image/log_out_white.png \
    image/menu (1).png \
    image/menu-4-xxl.png \
    image/menu.png \
    image/play-xxl.png \
    image/playWhite.png \
    image/profile.png \
    image/profile_white.png \
    image/settings.png \
    image/settings_white.png \
    image/statistics-xxl.png \
    logClient.txt

RESOURCES += \
    resource.qrc
