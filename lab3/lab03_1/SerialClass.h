#ifndef SERIALCLASS_H_INCLUDED
#define SERIALCLASS_H_INCLUDED

#define ARDUINO_WAIT_TIME 2000

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>

    //Initialize Serial communication with the given COM port
    void Serial(const char *portName);
    //Close the connection
    void _Serial();
    //Read data in a buffer, if nbChar is greater than the
    //maximum number of bytes available, it will return only the
    //bytes available. The function return -1 when nothing could
    //be read, the number of bytes actually read.
    int ReadData(char *buffer, unsigned int nbChar);
    //Writes data from a buffer through the Serial connection
    //return true on success.
    int WriteData(const char *buffer, unsigned int nbChar);
    //Check if we are actually connected
    int IsConnected();
    int DataRead();

#elif defined(__APPLE__) && defined(__MACH__)
    #include <stdint.h>
    #include <unistd.h>
    #include <termios.h>
    #include <glob.h>
    #include <fcntl.h>

    static inline void Sleep(unsigned int milliseconds)
    {
        usleep(milliseconds * 1000);
    }

    //Initialize Serial communication with the given COM port
    void Serial(const char *portName);
    //Close the connection
    void _Serial();
    //Read data in a buffer, if nbChar is greater than the
    //maximum number of bytes available, it will return only the
    //bytes available. The function return -1 when nothing could
    //be read, the number of bytes actually read.
    int ReadData(char *buffer, unsigned int nbChar);
    //Writes data from a buffer through the Serial connection
    //return true on success.
    int WriteData(const char *buffer, unsigned int nbChar);
    //Check if we are actually connected
    int IsConnected();
    int DataRead();
#endif // OS checks

#endif // SERIALCLASS_H_INCLUDED