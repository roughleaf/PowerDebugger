#include "serial_thread.h"
#include "datatypes.h"
#include "calculations.h"
#include "imgui.h"

void SerialLoop(std::shared_ptr<Args> args, std::shared_ptr<bool> enableLoop)
{
    static float t = 0;

    args->calabrationData.PrimaryV_M = 0.0116069;    // Got value from maual calibarion procedure. TODO Create calibration module
    args->calabrationData.PrimaryV_B = -1.15805;
    args->calabrationData.PrimaryI75R_M = 0.52;      // Result in uA
    args->calabrationData.PrimaryI75R_B = -3.5;
    args->calabrationData.PrimaryI10R_M = 4.4;
    args->calabrationData.PrimaryI10R_B = -26.2;
    args->calabrationData.PrimaryI1R_M = 30.7;
    args->calabrationData.PrimaryI1R_B = 850;

    args->calabrationData.AuxV_M = 0.0115274;
    args->calabrationData.AuxV_B = -0.9085;
    args->calabrationData.AuxI75R_M = 0.528;
    args->calabrationData.AuxI75R_B = -2.82;
    args->calabrationData.AuxI10R_M = 4.54;
    args->calabrationData.AuxI10R_B = -27.768;
    args->calabrationData.AuxI1R_M = 37.319;
    args->calabrationData.AuxI1R_B = 264.43;

	while (*enableLoop)
	{
		args->rawData = args->serialPort.GetRawData();

        // Convert the raw data to usable data units
        args->calculatedValues.AuxV = GetSingleValue(args->rawData.AuxVolt, args->calabrationData.AuxV_M, args->calabrationData.AuxV_B);
        args->calculatedValues.PrimaryV = GetSingleValue(args->rawData.PrimaryVolt, args->calabrationData.PrimaryV_M, args->calabrationData.PrimaryV_B);

        switch (args->rawData.AuxShunts)
        {
        case 0:
            args->calculatedValues.AuxI = GetSingleValue(args->rawData.AuxCurrent, args->calabrationData.AuxI75R_M, args->calabrationData.AuxI75R_B);
            break;
        case 1:
            args->calculatedValues.AuxI = GetSingleValue(args->rawData.AuxCurrent, args->calabrationData.AuxI10R_M, args->calabrationData.AuxI10R_B);
            break;
        case 2:
            args->calculatedValues.AuxI = GetSingleValue(args->rawData.AuxCurrent, args->calabrationData.AuxI1R_M, args->calabrationData.AuxI1R_B);
            break;
        case 3:
            args->calculatedValues.AuxI = GetSingleValue(args->rawData.AuxCurrent, args->calabrationData.AuxIPt_M, args->calabrationData.AuxIPt_B);
            break;
        default:
            break;
        }

        switch (args->rawData.PrimaryShunts)
        {
        case 0:
            args->calculatedValues.PrimaryI = GetSingleValue(args->rawData.PrimaryCurrent, args->calabrationData.PrimaryI75R_M, args->calabrationData.PrimaryI75R_B);
            break;
        case 1:
            args->calculatedValues.PrimaryI = GetSingleValue(args->rawData.PrimaryCurrent, args->calabrationData.PrimaryI10R_M, args->calabrationData.PrimaryI10R_B);
            break;
        case 2:
            args->calculatedValues.PrimaryI = GetSingleValue(args->rawData.PrimaryCurrent, args->calabrationData.PrimaryI1R_M, args->calabrationData.PrimaryI1R_B);
            break;
        case 3:
            args->calculatedValues.PrimaryI = GetSingleValue(args->rawData.PrimaryCurrent, args->calabrationData.PrimaryIPt_M, args->calabrationData.PrimaryIPt_B);
            break;
        default:
            break;
        }

        // Get the data to be plotted for the primary channel
        if (args->plotData.primaryVoltage.size() > (args->plotData.history*1000))
        {
            args->plotData.primaryVoltage.erase(args->plotData.primaryVoltage.begin(),
                args->plotData.primaryVoltage.begin() + (args->plotData.primaryVoltage.size() - (args->plotData.history * 1000)));
        }
        args->plotData.primaryVoltage.push_back(args->calculatedValues.PrimaryV);

        if (args->plotData.primaryCurrent.size() > (args->plotData.history * 1000))
        {
            args->plotData.primaryCurrent.erase(args->plotData.primaryCurrent.begin(),
                args->plotData.primaryCurrent.begin() + (args->plotData.primaryCurrent.size() - (args->plotData.history * 1000)));
        }
        args->plotData.primaryCurrent.push_back(args->calculatedValues.PrimaryI * 0.000001);

        if (args->plotData.primaryPower.size() > (args->plotData.history * 1000))
        {
            args->plotData.primaryPower.erase(args->plotData.primaryPower.begin(),
                args->plotData.primaryPower.begin() + (args->plotData.primaryPower.size() - (args->plotData.history * 1000)));
        }
        args->plotData.primaryPower.push_back(args->calculatedValues.PrimaryV * args->calculatedValues.PrimaryI * 0.000001);

        // Get the data to be plotted for the aux channel
        if (args->plotData.auxVoltage.size() > (args->plotData.history * 1000))
        {
            args->plotData.auxVoltage.erase(args->plotData.auxVoltage.begin(),
                args->plotData.auxVoltage.begin() + (args->plotData.auxVoltage.size() - (args->plotData.history * 1000)));
        }
        args->plotData.auxVoltage.push_back(args->calculatedValues.AuxV);

        if (args->plotData.auxCurrent.size() > (args->plotData.history * 1000))
        {
            args->plotData.auxCurrent.erase(args->plotData.auxCurrent.begin(),
                args->plotData.auxCurrent.begin() + (args->plotData.auxCurrent.size() - (args->plotData.history * 1000)));
        }
        args->plotData.auxCurrent.push_back(args->calculatedValues.AuxI * 0.000001);

        if (args->plotData.auxPower.size() > (args->plotData.history * 1000))
        {
            args->plotData.auxPower.erase(args->plotData.auxPower.begin(),
                args->plotData.auxPower.begin() + (args->plotData.auxPower.size() - (args->plotData.history * 1000)));
        }
        args->plotData.auxPower.push_back(args->calculatedValues.AuxV * args->calculatedValues.AuxI * 0.000001);
	}
}
