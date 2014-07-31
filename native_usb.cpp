#include "native_usb.h"
#include <QByteArray>

native_usb::native_usb(QObject *parent) :
    QObject(parent)
{
}

int native_usb::open_usb(int vid, int pid)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    int r;
    usb = NULL;

    r = libusb_init(NULL);
    if (r < 0)
        return -1;

    libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_INFO);

    qDebug("Opening the USB device...");
    usb = libusb_open_device_with_vid_pid(NULL, vid, pid);
    if(usb == NULL)
    {
        qDebug("Error opening device");
        return -1;
    }

    r = libusb_claim_interface(usb, 0);
    if(r != 0)
    {
        qDebug("Claim Interface: %s", libusb_error_name(r));
        return -1;
    }

    qDebug("Releasing interface and closing the USB device...");
    libusb_release_interface(usb, 0);
    libusb_close(usb);
    usb = NULL;

    Sleep(500);

    qDebug("Reopening the USB device...");
    usb = libusb_open_device_with_vid_pid(NULL, 0x0da4, 0x0008);
    if(usb == NULL)
    {
        qDebug("Error opening device");
        return -1;
    }

    r = libusb_claim_interface(usb, 0);
    if(r != 0)
    {
        qDebug("Reclaim Interface: %s", libusb_error_name(r));
        return -1;
    }

    Sleep(500);

    return 0;
#endif
#if defined(Q_OS_MAC)
    return 0;
#endif
}

int native_usb::write_usb(QByteArray packet)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    int actual_length;
    QByteArray correct_packet;

    if(usb == NULL)
        return -1;

    if(packet.length() > 64)
        return -1;

    correct_packet.resize(64 - packet.length());
    correct_packet.fill(0x00);
    correct_packet.prepend(packet);

    libusb_interrupt_transfer(usb, (1 | LIBUSB_ENDPOINT_OUT), (unsigned char *)correct_packet.constData(), correct_packet.length(), &actual_length, 0);

    return actual_length;
#endif
#if defined(Q_OS_MAC)
    return 0;
#endif
}

QByteArray native_usb::read_usb()
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    QByteArray packet;
    unsigned char char_packet[64];
    int actual_length;

    if(usb == NULL)
        return packet;

    memset(char_packet, 0x00, sizeof(char_packet));
    libusb_interrupt_transfer(usb, (1 | LIBUSB_ENDPOINT_IN), char_packet, sizeof(char_packet), &actual_length, 0);
    packet.append((char *)char_packet, actual_length);

    return packet;
#endif
#if defined(Q_OS_MAC)
    return NULL;
#endif
}

int native_usb::close_usb()
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    int r;

    if(usb == NULL)
        return -1;

    r = libusb_release_interface(usb, 0);
    if(r != 0)
    {
        qDebug("Release interface: %s", libusb_error_name(r));
        return -1;
    }

    libusb_close(usb);
    libusb_exit(NULL);

   return 0;
#endif
#if defined(Q_OS_MAC)
    return 0;
#endif
}