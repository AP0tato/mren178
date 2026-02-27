#include "SerialClass.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include <fcntl.h>
#include <unistd.h>
#endif

#include <string.h>

#define false 0
#define true 1

#if defined(_WIN32) || defined(_WIN64)
//Serial comm handler
HANDLE hSerial;
//Connection status
int connected;
//Get various information about the connection
COMSTAT status;
//Keep track of last error
DWORD errors;

void Serial(const char *portName)
{
    //We're not yet connected
    connected = false;

    //Try to connect to the given port throuh CreateFile
    hSerial = CreateFile(portName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    //Check if the connection was successfull
    if(hSerial==INVALID_HANDLE_VALUE)
    {
        //If not success full display an Error
        if(GetLastError()==ERROR_FILE_NOT_FOUND){

            //Print Error if neccessary
            printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName);

        }
        else
        {
            printf("ERROR!!!");
        }
    }
    else
    {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = {0};

        //Try to get the current
        if (!GetCommState(hSerial, &dcbSerialParams))
        {
            //If impossible, show an error
            printf("Failed to get current serial parameters!\n");
        }
        else
        {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate=CBR_9600;
            dcbSerialParams.ByteSize=8;
            dcbSerialParams.StopBits=ONESTOPBIT;
            dcbSerialParams.Parity=NOPARITY;
            //Setting the DTR to Control_Enable ensures that the Arduino is properly
            //reset upon establishing a connection
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

             //Set the parameters and check for their proper application
             if(!SetCommState(hSerial, &dcbSerialParams))
             {
                printf("ALERT: Could not set serial port parameters.");
             }
             else
             {
                 //If everything went fine we're connected
                 connected = true;
                 //Flush any remaining characters in the buffers 
                 PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
                 //We wait 2s as the arduino board will be reseting
                 Sleep(ARDUINO_WAIT_TIME);
             }
        }
    }

}

void _Serial()
{
    //Check if we are connected before trying to disconnect
    if( connected)
    {
        //We're no longer connected
         connected = false;
        //Close the serial handler
        CloseHandle( hSerial);
    }
}

int  ReadData(char *buffer, unsigned int nbChar)
{
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError( hSerial, & errors, & status);

    //Check if there is something to read
    if( status.cbInQue>0)
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if( status.cbInQue>nbChar)
        {
            toRead = nbChar;
        }
        else
        {
            toRead =  status.cbInQue;
        }

        //Try to read the require number of chars, and return the number of read bytes on success
        if(ReadFile( hSerial, buffer, toRead, &bytesRead, NULL) )
        {
            return bytesRead;
        }

    }

    //If nothing has been read, or that an error was detected return 0
    return 0;

}


int WriteData(const char *buffer, unsigned int nbChar)
{
    DWORD bytesSend;

    //Try to write the buffer on the Serial port
    if(!WriteFile( hSerial, (void *)buffer, nbChar, &bytesSend, 0))
    {
        //In case it don't work get comm error and return false
        ClearCommError( hSerial, & errors, & status);

        return false;
    }
    else
        return true;
}

int IsConnected()
{
    //Simply return the connection status
    return  connected;
}

int DataRead(){
    char incomingData[8] = "";			// pre-allocate memory
    int dataLength = 7;
    int readResult = 0;
    int num;//storing the value read from the sensor

        while( IsConnected())
        {
            readResult =  ReadData(incomingData,dataLength);
            //printf("Bytes read: (0 means no data available) %i\n",readResult);
            incomingData[readResult] = 0;

        
            char *p = strtok(incomingData,"\n");
            char n[20];
            while(p != NULL){
                strcpy(n,p);
                p = strtok(NULL,"\n");
                num = atoi(n);
                printf("Value read from sensor: %d\n",num);
            }
            if(num) break;
        }
    return num;
}

#elif defined(__APPLE__) && defined(__MACH__)

#ifndef O_NOCTTY
#define O_NOCTTY 0
#endif

int connected;
uint32_t errors;
static int serialFd = -1;

//Initialize Serial communication with the given COM port
void Serial(const char *portName)
{
    connected = false;
    serialFd = open(portName, O_RDWR | O_NOCTTY);
    if(serialFd < 0)
    {
        perror("open");
        return;
    }

    struct termios tty;
    if (tcgetattr(serialFd, &tty) != 0)
    {
        perror("tcgetattr");
        close(serialFd);
        serialFd = -1;
        return;
    }

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(serialFd, TCSANOW, &tty) != 0)
    {
        perror("tcsetattr");
        close(serialFd);
        serialFd = -1;
        return;
    }

    connected = true;
    Sleep(ARDUINO_WAIT_TIME);
}

//Close the connection
void _Serial()
{
    if (serialFd >= 0)
    {
        close(serialFd);
        serialFd = -1;
    }
    connected = false;
}

//Read data in a buffer, if nbChar is greater than the
//maximum number of bytes available, it will return only the
//bytes available. The function return -1 when nothing could
//be read, the number of bytes actually read.
int ReadData(char *buffer, unsigned int nbChar)
{
    if (!connected || serialFd < 0 || buffer == NULL || nbChar == 0)
    {
        return 0;
    }

    ssize_t n = read(serialFd, buffer, nbChar);
    if (n > 0)
    {
        return (int)n;
    }

    return 0;
}

//Writes data from a buffer through the Serial connection
//return true on success.
int WriteData(const char *buffer, unsigned int nbChar)
{
    if (!connected || serialFd < 0 || buffer == NULL || nbChar == 0)
    {
        return false;
    }

    ssize_t written = write(serialFd, buffer, nbChar);
    return (written == (ssize_t)nbChar) ? true : false;
}

//Check if we are actually connected
int IsConnected()
{
    return connected;
}

int DataRead()
{
    char incomingData[8] = "";
    int dataLength = 7;
    int readResult = 0;
    int num = 0;

    while(IsConnected())
    {
        readResult = ReadData(incomingData, dataLength);
        if (readResult <= 0)
        {
            continue;
        }

        if (readResult >= dataLength)
        {
            readResult = dataLength - 1;
        }
        incomingData[readResult] = 0;

        char *p = strtok(incomingData, "\n");
        char n[20];
        while(p != NULL)
        {
            strcpy(n, p);
            p = strtok(NULL, "\n");
            num = atoi(n);
            printf("Value read from sensor: %d\n", num);
        }
        if(num)
        {
            break;
        }
    }

    return num;

}

#endif

