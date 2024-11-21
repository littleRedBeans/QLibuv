QT = core
TEMPLATE = lib
CONFIG += c++17 static

DESTDIR = $$_PRO_FILE_PWD_/../lib

LIBS += -luv

INSTALLS_HEADERS_PATH = $$_PRO_FILE_PWD_/../include/QLibuv
headers.files += $$_PRO_FILE_PWD_/UvUdpSocket.h $$_PRO_FILE_PWD_/UvTcpServer.h \
	$$_PRO_FILE_PWD_/Callbacks.h
headers.path = $$INSTALLS_HEADERS_PATH

INSTALLS += headers

HEADERS += \
	Callbacks.h \
	EventDispatcherLibUv.h \
	EventDispatcherLibUvPrivate.h \
	NetManager.h \
	UvTcpServer.h \
	UvUdpSocket.h

SOURCES += \
	EventDispatcherLibUv.cpp \
	EventDispatcherLibUvPrivate.cpp \
	NetManager.cpp \
	UvTcpServer.cpp \
	UvUdpSocket.cpp
