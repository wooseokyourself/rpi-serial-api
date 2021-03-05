#include "../include/Serial.hpp"

Serial::Serial () { }

Serial::Serial (const char* port, const int baudRate) {
    struct termios options;
    speed_t myBaud;
    int     status;

    switch (baudRate) {
        case      50:	myBaud =      B50 ; break ;
        case      75:	myBaud =      B75 ; break ;
        case     110:	myBaud =     B110 ; break ;
        case     134:	myBaud =     B134 ; break ;
        case     150:	myBaud =     B150 ; break ;
        case     200:	myBaud =     B200 ; break ;
        case     300:	myBaud =     B300 ; break ;
        case     600:	myBaud =     B600 ; break ;
        case    1200:	myBaud =    B1200 ; break ;
        case    1800:	myBaud =    B1800 ; break ;
        case    2400:	myBaud =    B2400 ; break ;
        case    4800:	myBaud =    B4800 ; break ;
        case    9600:	myBaud =    B9600 ; break ;
        case   19200:	myBaud =   B19200 ; break ;
        case   38400:	myBaud =   B38400 ; break ;
        case   57600:	myBaud =   B57600 ; break ;
        case  115200:	myBaud =  B115200 ; break ;
        case  230400:	myBaud =  B230400 ; break ;
        case  460800:	myBaud =  B460800 ; break ;
        case  500000:	myBaud =  B500000 ; break ;
        case  576000:	myBaud =  B576000 ; break ;
        case  921600:	myBaud =  B921600 ; break ;
        case 1000000:	myBaud = B1000000 ; break ;
        case 1152000:	myBaud = B1152000 ; break ;
        case 1500000:	myBaud = B1500000 ; break ;
        case 2000000:	myBaud = B2000000 ; break ;
        case 2500000:	myBaud = B2500000 ; break ;
        case 3000000:	myBaud = B3000000 ; break ;
        case 3500000:	myBaud = B3500000 ; break ;
        case 4000000:	myBaud = B4000000 ; break ;

        default:
        std::cerr << "Serial: " << "undefinde baud rate" << std::endl;
    }

    if ((this->fd = open (port, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
        std::cerr << "Serial: " << "can't open the port: " << port << std::endl;

    fcntl (this->fd, F_SETFL, O_RDWR);

    // Get and modify current options:

    tcgetattr (this->fd, &options);

        cfmakeraw   (&options);
        cfsetispeed (&options, myBaud);
        cfsetospeed (&options, myBaud);

        options.c_cflag |= (CLOCAL | CREAD);
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_oflag &= ~OPOST;

        options.c_cc [VMIN]  =   0 ;
        options.c_cc [VTIME] = 100 ;	// Ten seconds (100 deciseconds)

    tcsetattr(this->fd, TCSANOW, &options);

    ioctl(this->fd, TIOCMGET, &status);

    status |= TIOCM_DTR;
    status |= TIOCM_RTS;

    ioctl(this->fd, TIOCMSET, &status);

    usleep(10000);	// 10mS
}

Serial::~Serial () {
    this->release();
}

/**
 * @brief Flush the serial buffers. (both tx & rx)
 */
void Serial::flush () {
    tcflush(this->fd, TCIOFLUSH);
}

/**
 * @brief Return the number of bytes of data available to be read in the serial port.
 */
int Serial::remaining () {
    int len;
    if (ioctl(this->fd, FIONREAD, &len) == -1)
        return -1;
    return len;
}

/**
 * @brief Send a string to the serial port.
 * @param s It shouldn't have null as a byte.
 */
void Serial::puts (const char* s) {
    write (this->fd, s, strlen(s));
}

/**
 * @brief Send a string to the serial port.
 * @param s It can have null as a byte.
 * @param len The number of bytes from s.
 */
void Serial::puts (const char* s, const size_t len) {
    write (this->fd, s, len);
}

/**
 * @brief Get a single character from the serial device.
 *          Note: Zero is a valid character and this function will time-out after 10 secs.
 * @return Character.
 */ 
int Serial::getchar () {
    uint8_t x;
    if (read(this->fd, &x, 1) != 1)
        return -1;
    return ((int)x) & 0xFF;
}

/**
 * @brief Release the serial port.
 */
void Serial::release () {
    close(this->fd);
    this->fd = -1;
}