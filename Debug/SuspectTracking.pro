QT       += core gui axcontainer charts sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Dbhelper.cpp \
    all_i.cpp \
    analy.cpp \
    date.cpp \
    edge.cpp \
    excelengine.cpp \
    excelnode.cpp \
    graphwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    money.cpp \
    node.cpp \
    paywidget.cpp \
    query.cpp \
    query_fir.cpp \
    query_sec.cpp \
    query_thi.cpp \
    telwidget.cpp \
    themewidget.cpp \
    user_i.cpp \
    user_s.cpp

HEADERS += \
    Dbhelper.h \
    all_i.h \
    analy.h \
    date.h \
    edge.h \
    excelengine.h \
    excelnode.h \
    graphwidget.h \
    mainwindow.h \
    money.h \
    node.h \
    paywidget.h \
    query.h \
    query_fir.h \
    query_sec.h \
    query_thi.h \
    telwidget.h \
    themewidget.h \
    user_i.h \
    user_s.h

FORMS += \
    all_i.ui \
    analy.ui \
    date.ui \
    mainwindow.ui \
    money.ui \
    paywidget.ui \
    query.ui \
    query_fir.ui \
    query_sec.ui \
    query_thi.ui \
    telwidget.ui \
    themewidget.ui \
    user_i.ui \
    user_s.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    first_star.ico
RC_ICONS = first_star.ico

RESOURCES += \
    icon.qrc \
    res.qrc
