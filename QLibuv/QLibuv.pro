QT = core
TEMPLATE = lib
CONFIG += c++17 static

DESTDIR = $$_PRO_FILE_PWD_/../lib

LIBS += -luv

INSTALLS_HEADERS_PATH = $$_PRO_FILE_PWD_/../include/QLibuv
headers.files += $$_PRO_FILE_PWD_/UvUdpSocket.h
headers.path = $$INSTALLS_HEADERS_PATH

INSTALLS += headers

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
