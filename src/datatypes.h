#pragma once
#include <serial_comms.h>
#include "imgui.h"
#include <vector>

struct CalibrationData
{
	double PrimaryV_M;
	double PrimaryV_B;
	double PrimaryI75R_M;
	double PrimaryI75R_B;
	double PrimaryI10R_M;
	double PrimaryI10R_B;
	double PrimaryI1R_M;
	double PrimaryI1R_B;
	double PrimaryIPt_M;
	double PrimaryIPt_B;

	double AuxV_M;
	double AuxV_B;
	double AuxI75R_M;
	double AuxI75R_B;
	double AuxI10R_M;
	double AuxI10R_B;
	double AuxI1R_M;
	double AuxI1R_B;
	double AuxIPt_M;
	double AuxIPt_B;
};

struct CalculatedValues
{
	float PrimaryV;
	float PrimaryI;
	float AuxV;
	float AuxI;
};

// Helper struct to add datapoints to graph
// Copied and slightly modified from ScrollingBuffer in implot_demo.cpp
struct PlotBuffer 
{
	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;
	PlotBuffer(int max_size = 2000)
	{
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y)
	{
		if (Data.size() < MaxSize)
			Data.push_back(ImVec2(x, y));
		else
		{
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase()
	{
		if (Data.size() > 0)
		{
			Data.shrink(0);
			Offset = 0;
		}
	}
};

struct PlotData
{
	/*PlotBuffer primaryVoltage;
	PlotBuffer primaryCurrent;
	PlotBuffer primaryPower;
	PlotBuffer auxVoltage;
	PlotBuffer auxCurrent;
	PlotBuffer auxPower;*/
	std::vector<float> primaryVoltage;
	std::vector<float> primaryCurrent;
	std::vector<float> primaryPower;
	std::vector<float> auxVoltage;
	std::vector<float> auxCurrent;
	std::vector<float> auxPower;

	float history;

	float t;
};

struct Args
{
	RawData rawData;
	SerialPort serialPort;
	CalculatedValues calculatedValues;
	CalibrationData calabrationData;
	PlotData plotData;
};

struct ConfigData
{
	bool portIsOpen;
	char* comboPortItems[15];
	int selectedPort;
	std::vector<std::string> serialPortList;
	std::vector<std::string> serialPortListFull;
};