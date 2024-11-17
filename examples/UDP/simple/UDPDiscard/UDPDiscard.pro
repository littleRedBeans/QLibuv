QT = core

CONFIG += c++17 cmdline
#QMAKE_CXXFLAGS+="-fsanitize=address"
#QMAKE_CFLAGS+="-fsanitize=address"
#QMAKE_LFLAGS+="-fsanitize=address"
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DESTDIR = $$_PRO_FILE_PWD_/../../../../bin
INCLUDEPATH += $$_PRO_FILE_PWD_/../../../../include

LIBS += -L$$_PRO_FILE_PWD_/../../../../lib
LIBS += -luv -lQLibuv
SOURCES += \
        DiscardServer.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	DiscardServer.h
