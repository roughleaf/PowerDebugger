#include <string>

#include "serial_comms.h"

SerialPort::SerialPort(void)
{
}

SerialPort::~SerialPort(void)
{
    if (_portIsOpen)
        ClosePort();
}

std::vector<int> SerialPort::GetAvailablePorts()
{
    char lpTargetPath[5000]; // buffer to store the path of the COM PORTS
    std::vector<int> portList;

    for (int i = 0; i < 255; i++) // checking ports from COM0 to COM255
    {
        std::string str = "COM" + std::to_string(i); // converting to COM0, COM1, COM2
        DWORD res = QueryDosDevice(str.c_str(), lpTargetPath, 5000);

        // Test the return value and error if any
        if (res != 0) //QueryDosDevice returns zero if it didn't find an object
        {
            portList.push_back(i);
            //std::cout << str << ": " << lpTargetPath << std::endl;
        }
        if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
        }
    }
    return portList;
}

int SerialPort::OpenPort(std::string sPort)
{
    if (!_portIsOpen)
    {
        std::string temp = std::string(sPort.begin(), sPort.end());
        LPCSTR szPort = temp.c_str();

        _serialHandle = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if (!_serialHandle)
            return -1;

        // Do some basic settings
        DCB serialParams{};
        //serialParams.DCBlength = sizeof(serialParams);

        GetCommState(_serialHandle, &serialParams);
        serialParams.BaudRate = 230400;
        serialParams.ByteSize = 8;
        serialParams.StopBits = ONESTOPBIT;
        serialParams.Parity = NOPARITY;
        SetCommState(_serialHandle, &serialParams);

        // Set timeouts
        COMMTIMEOUTS timeout = { 0 };
        timeout.ReadIntervalTimeout = 50;
        timeout.ReadTotalTimeoutConstant = 50;
        timeout.ReadTotalTimeoutMultiplier = 10;
        timeout.WriteTotalTimeoutConstant = 50;
        timeout.WriteTotalTimeoutMultiplier = 10;

        if (!SetCommTimeouts(_serialHandle, &timeout))
            return -2;

        if (!SetCommMask(_serialHandle, EV_RXCHAR))
            return -3;

        _portIsOpen = true;
    }
    return 1;
}

bool SerialPort::ClosePort()
{
    if (_portIsOpen)
    {
        if (CloseHandle(_serialHandle))
        {
            _portIsOpen = false;
            return true;
        }
        return false;
    }
}

int SerialPort::WriteByte(char toWrite)
{
    char rxBuf[1];
    rxBuf[0] = toWrite;
    //rxBuf[1] = toWrite;
    if (_serialHandle)
    {
        LPDWORD bytesWritten = 0;
        WriteFile(_serialHandle, &toWrite, 1, bytesWritten, NULL);
        return 0;
    }
    return -1;
}

RawData SerialPort::GetRawData()
{
    if (!_portIsOpen)
        return _rawData;
    //char buf[9]{};
    //char buf1[9]{};
    uint8_t buffer[BUFFER_SIZE]{};

    int bytesToRead = PACKET_SIZE;
    int bytesRead{};

    DWORD lpEvtMask{};
    //LPDWORD bytesRead = 0;

    if (WaitCommEvent(_serialHandle, &lpEvtMask, NULL))
    {
        if (read(buffer, 0, 1))
        {
            if ((buffer[0] & 0x7F) == '<') // Test for start char. Discard packet if not found to ensure data sync
            {
                _rawData.PrimaryOut = buffer[0] >> 7; // Read the MSB bit
                do
                {
                    bytesRead += read(buffer, bytesRead + 1, bytesToRead);
                    if (bytesRead < 0)
                    {
                        _rawData = {};
                        return _rawData;
                    }
                    bytesToRead -= bytesRead;
                } while (bytesToRead > 0);

                if ((buffer[8] & 0x7F) == '>')       // Test for end char. Discard packet if not found to ensure data sync
                {
                    _rawData.PrimarySource0 = buffer[1] >> 7;
                    _rawData.PrimarySource1 = (buffer[1] >> 6) & 1;
                    _rawData.PrimaryOn = (buffer[1] >> 5) & 1;
                    _rawData.AuxOn = (buffer[1] >> 4) & 1;
                    _rawData.PrimaryShunts = (buffer[1] >> 2) & 3;
                    _rawData.AuxShunts = buffer[1] & 3;


                    _rawData.AuxVolt = (buffer[2] << 4) + ((buffer[3] & 0xF0) >> 4);
                    _rawData.AuxCurrent = ((buffer[3] & 0x0F) << 8) + buffer[4];

                    _rawData.PrimaryVolt = (buffer[5] << 4) + (buffer[6] >> 4);
                    _rawData.PrimaryCurrent = ((buffer[6] & 0x0F) << 8) + buffer[7];

                    _rawData.DebugMode = buffer[8] >> 7; // Read the MSB bit
                }
            }
        }
    }

    if (_txQueue != 0)
    {
        WriteByte(_txQueue);
        _txQueue = 0;
    }

    return _rawData;
}

int SerialPort::AddCharToTxQueue(const char txQueue)
{
    if(_txQueue != 0)
        return -1;

    _txQueue = txQueue;
    return 1;
}

int SerialPort::read(uint8_t* buffer, int offset, int count)
{
    DWORD readCount{};

    uint8_t buff[10]{};

    if (ReadFile(_serialHandle, buff, count, &readCount, NULL) != 0)
    {
        //strcat_s(buf, 10, buff);
        memcpy_s((buffer + offset), 9, buff, readCount);

        return static_cast<int>(readCount);
    }

    return -1;
}



