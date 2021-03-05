#ifndef __SERIAL__
#define __SERIAL__

/**
 * reference: https://github.com/WiringPi/WiringPi/blob/master/wiringPi/wiringSerial.c
 */

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstdarg>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

class Serial {
public:
    Serial ();
    Serial (const char* port, const int baudRate);
    ~Serial ();

protected:
    void flush ();
    int remaining ();
    void puts (const char* s);
    void puts (const char* s, const size_t len);
    int getchar ();
    void release ();

protected:
    int fd;
};

#endif