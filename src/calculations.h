#pragma once

struct CalData
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

float GetSingleValue(int raw, double m, double b);