INCLUDEPATH += $$PWD
VPATH += $$PWD

HEADERS += native_usb.h
SOURCES += native_usb.cpp

win32 {
    INCLUDEPATH += $$PWD/../../../binary/win/libusb-1.0
    LIBS += $$PWD/../../../binary/win/libusb-1.0/libusb-1.0.a
}

linux {
    INCLUDEPATH += $$PWD/../../../binary/linux/libusb-1.0
    LIBS += $$PWD/../../../binary/linux/libusb-1.0/libusb-1.0.a -ludev
}

macx {
    include(osx/osx.pri)
}
