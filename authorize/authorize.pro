QT       += core gui

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
    main.cpp \
    dialog.cpp \
    mainwindow.cpp

HEADERS += \
    dialog.h \
    mainwindow.h

FORMS += \
    dialog.ui \
    mainwindow.ui

# 子工程
include ($$PWD/utils/utils.pri)

# 附加包含路径
INCLUDEPATH += \
        $$PWD/libs/auth \
        $$PWD/libs/inviter \
        $$PWD/libs/authlog

# 从文件读取版本号
#CAT_VERSION = $$cat($$PWD/version)

# 拆分出版本号
#VERSION_MAJOR = $$section(CAT_VERSION, ., 0, 0)
#VERSION_MINOR = $$section(CAT_VERSION, ., 1, 1)
#VERSION_PATCH = $$section(CAT_VERSION, ., 2, 2)
#message("version:" $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH})

# qmake变量的方式定义版本号
#VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}

# ***********************************************************
# Win平台下配置
# ***********************************************************
win32 {
    # 通过rc的方式的话，VERSION变量rc中获取不到,定义为宏方便rc中使用
    #DEFINES += VERSION_MAJOR=$${VERSION_MAJOR}
    #DEFINES += VERSION_MINOR=$${VERSION_MINOR}
    #DEFINES += VERSION_PATCH=$${VERSION_PATCH}
    #DEFINES += VERSION_RC_STR=\\\"$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}\\\"

    contains(QT_ARCH, x86_64) {
        message("x64")
        # 输出目录
        CONFIG(debug, debug|release) {
            DESTDIR = $$PWD/../output/win/x64/debug
        } else {
            DESTDIR = $$PWD/../output/win/x64/release
        }
    } else {
        message("x86")
        # 输出目录
        CONFIG(debug, debug|release) {
            DESTDIR = $$PWD/../output/win/x86/debug
        } else {
            DESTDIR = $$PWD/../output/win/x86/release
        }
    }

    # 复制依赖库
    WIN_AUTH_SRC = $$PWD/../libs/auth/*.dll
    WIN_INVITER_SRC = $$PWD/../libs/inviter/*.dll
    WIN_AUTHLOG_SRC = $$PWD/../libs/authlog/*.dll
    WIN_DST = $$DESTDIR

    WIN_AUTH_SRC ~= s,/,\\,g
    WIN_INVITER_SRC ~= s,/,\\,g
    WIN_AUTHLOG_SRC ~= s,/,\\,g
    WIN_DST ~= s,/,\\,g

    QMAKE_POST_LINK += $$quote($$QMAKE_COPY $$WIN_AUTH_SRC $$WIN_DST$$escape_expand(\n\t))
    QMAKE_POST_LINK += $$quote($$QMAKE_COPY $$WIN_INVITER_SRC $$WIN_DST$$escape_expand(\n\t))
    QMAKE_POST_LINK += $$quote($$QMAKE_COPY $$WIN_AUTHLOG_SRC $$WIN_DST$$escape_expand(\n\t))

    RC_FILE = $$PWD/res/authorize.rc
}

#ROOT_DIR = $$PWD/..
#ROOT_DIR ~= s,/,\\,g
#QMAKE_POST_LINK += $$quote($$QMAKE_COPY $$ROOT_DIR\libs\auth\*.dll $$WIN_DST$$escape_expand(\n\t))
