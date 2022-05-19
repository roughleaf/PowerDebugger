#pragma once

#include <Windows.h>
#include <vector>
#include <string>

constexpr const int PACKET_SIZE = 8;	// Exclude the start byte from the packed size
constexpr const int BUFFER_SIZE = 10;	// received packes is 9 bytes + null terminating bytes = 10 bytes

struct RawData
{
	int PrimaryVolt;
	int PrimaryCurrent;
	int AuxVolt;
	int AuxCurrent;
	bool PrimaryOut;
	bool PrimarySource0;
	bool PrimarySource1;
	bool PrimaryOn;
	bool AuxOn;
	int PrimaryShunts;
	int AuxShunts;
	bool DebugMode;
};

class SerialPort
{
public:
	typedef union
	{
		unsigned short s_int;
		uint8_t bytes[2];
	} BytesToUint8;

	SerialPort(void);
	~SerialPort(void);
	std::vector<int> GetAvailablePorts();
	int OpenPort(std::string sPort);
	bool ClosePort();
	int WriteByte(char toWrite);
	RawData GetRawData();
	int AddCharToTxQueue(const char txQueue);

private:
	char _txQueue{ 0 };
	RawData _rawData{};
	BytesToUint8 _bytesToUint8{};
	HANDLE _serialHandle;
	bool _portIsOpen{false};
	int read(uint8_t* buffer, int offset, int count);
}; // SerialPort Class