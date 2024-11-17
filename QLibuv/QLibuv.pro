QT = core
TEMPLATE = lib
CONFIG += c++17 static

DESTDIR = $$_PRO_FILE_PWD_/../lib

LIBS += -luv

unix{
    QMAKE_POST_LINK += /bin/bash $$_PRO_FILE_PWD_/moveHeader.sh $$_PRO_FILE_PWD_
}

HEADERS += \
	EventDispatcherLibUv.h \
	EventDispatcherLibUvPrivate.h \
	NetManager.h \
	UvUdpSocket.h

SOURCES += \
	EventDispatcherLibUv.cpp \
	EventDispatcherLibUvPrivate.cpp \
	NetManager.cpp \
	UvUdpSocket.cpp
