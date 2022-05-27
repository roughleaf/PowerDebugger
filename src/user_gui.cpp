#include "imgui.h"
#include "user_gui.h"
#include "datatypes.h"
#include "implot.h"
#include "implot_internal.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "imgui_impl_glfw.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Copied from implot_demo.cpp. Credit to original author
void MetricFormatter(double value, char* buff, int size, void* data) {
    const char* unit = (const char*)data;
    static double v[] = { 1000000000,1000000,1000,1,0.001,0.000001,0.000000001 };
    static const char* p[] = { "G","M","k","","m","u","n" };
    if (value == 0) {
        snprintf(buff, size, "0 %s", unit);
        return;
    }
    for (int i = 0; i < 7; ++i) {
        if (fabs(value) >= v[i]) {
            snprintf(buff, size, "%g %s%s", value / v[i], p[i], unit);
            return;
        }
    }
    snprintf(buff, size, "%g %s%s", value / v[6], p[6], unit);
}

namespace UserGui
{
    void ShowUserWindow(GLFWwindow *window ,std::shared_ptr<Args> args, std::shared_ptr<ConfigData> configData)
    {
        ShowConfigurationWindow(args, configData);
        ShowPlotWindow(args);
    }

    void ShowConfigurationWindow(std::shared_ptr<Args> args, std::shared_ptr<ConfigData> configData)
    {
        ImVec2 btnSize{ 100,20 };
        ImVec4 btnGreenOn{ 0, 0.65f, 0, 1 };
        ImVec4 btnGreenOnActive{ 0, 0.45f, 0, 1 };
        ImVec4 btnGreenOnHover{ 0, 0.55f, 0, 1 };

        ImVec4 btnRedOff{ 0.3f, 0, 0, 1 };
        ImVec4 btnRedOffActive{ 0.2f, 0, 0, 1 };
        ImVec4 btnRedOffHover{ 0.15f, 0, 0, 1 };

        static bool plotDemo = false;

        ImGui::Begin("Controls");
        ImGui::Checkbox("Show Plot Demo", &plotDemo);

        if (plotDemo)
        {
            ImPlot::ShowDemoWindow();
        }
        ImGui::BeginDisabled(configData->portIsOpen);
        ImGui::SetNextItemWidth(150);
        ImGui::Combo(" ", &configData->selectedPort, configData->comboPortItems, configData->serialPortList.size());
        ImGui::EndDisabled();
        //ImGui::SameLine();
        if (!configData->portIsOpen)
        {
            if (ImGui::Button("Open Port", ImVec2(150, 20)))
            {
                if (args->serialPort.OpenPort(configData->serialPortListFull[configData->selectedPort].c_str()))
                {
                    configData->portIsOpen = true;
                }
            }
        }
        else
        {
            if (ImGui::Button("Close Port", ImVec2(150, 20)))
            {
                if (args->serialPort.ClosePort())
                {
                    configData->portIsOpen = false;
                }
            }

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
            window_flags |= ImGuiWindowFlags_MenuBar;
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("Primary", ImVec2(200, 350), true, window_flags);

            // Primary On button group
            if (!args->rawData.PrimaryOn)
            {
                if (CustomColorButton("Primary", btnSize, btnRedOff, btnRedOffActive, btnRedOffHover))
                {
                    args->serialPort.AddCharToTxQueue('q');
                }
            }
            else
            {
                if (CustomColorButton("Primary", btnSize, btnGreenOn, btnGreenOnActive, btnGreenOnHover))
                {
                    args->serialPort.AddCharToTxQueue('w');
                }
            }

            ImGui::Separator();

            ImGui::Text("Primary Output");

            static int PrimaryDestination = args->rawData.PrimaryOut;
            if (ImGui::RadioButton("USB Out", &PrimaryDestination, 0))
            {
                args->serialPort.AddCharToTxQueue('t');
            }
            if (ImGui::RadioButton("Header Out", &PrimaryDestination, 1))
            {
                args->serialPort.AddCharToTxQueue('y');
            }

            ImGui::Separator();
            ImGui::Text("Primary Source");
            static int PrimarySource = (((int)args->rawData.PrimarySource1 << 1) + (int)args->rawData.PrimarySource0);

            if (ImGui::RadioButton("USB A", &PrimarySource, 0))
            {
                args->serialPort.AddCharToTxQueue('u');
            }
            if (ImGui::RadioButton("USB B", &PrimarySource, 2))
            {
                args->serialPort.AddCharToTxQueue('i');
            }
            if (ImGui::RadioButton("USB A 3.3V", &PrimarySource, 1))
            {
                args->serialPort.AddCharToTxQueue('o');
            }
            if (ImGui::RadioButton("USB B 3.3V", &PrimarySource, 3))
            {
                args->serialPort.AddCharToTxQueue('p');
            }


            ImGui::Text("Primary V: %d raw", args->rawData.PrimaryVolt);
            ImGui::Text("Primary V: %2.2fV", args->calculatedValues.PrimaryV);//GetSingleValue(measuredData->PrimaryVolt, _caldata.PrimaryV_M, _caldata.PrimaryV_B));
            ImGui::Text("Primary I: %d raw", args->rawData.PrimaryCurrent);
            ImGui::Text("Primary I: %2.2fuA", args->calculatedValues.PrimaryI);

            ImGui::EndChild();


            // Aux Groupings
            ImGui::BeginChild("Aux", ImVec2(200, 250), true, window_flags);

            if (!args->rawData.AuxOn)
            {
                if (CustomColorButton("Aux", btnSize, btnRedOff, btnRedOffActive, btnRedOffHover))
                {
                    args->serialPort.AddCharToTxQueue('e');
                }
            }
            else
            {
                if (CustomColorButton("Aux", btnSize, btnGreenOn, btnGreenOnActive, btnGreenOnHover))
                {
                    args->serialPort.AddCharToTxQueue('r');
                }
            }
            ImGui::Separator();
            ImGui::Text("Aux V:     %d raw", args->rawData.AuxVolt);
            ImGui::Text("Aux V:     %2.2fV", args->calculatedValues.AuxV);//GetSingleValue(measuredData->AuxVolt, _caldata.AuxV_M, _caldata.AuxV_B));
            ImGui::Text("Aux I:     %d raw", args->rawData.AuxCurrent);
            ImGui::Text("Aux I:     %2.2fuA", args->calculatedValues.AuxI);
            ImGui::EndChild();

            if (ImGui::Checkbox("Enable Debug", &args->rawData.DebugMode))
            {
                switch (args->rawData.DebugMode)
                {
                case true:
                    args->serialPort.AddCharToTxQueue('n');
                    break;
                case false:
                    args->serialPort.AddCharToTxQueue('m');
                    break;
                default:
                    break;
                }
            }

            if (args->rawData.DebugMode)
            {
                ShowShuntDebugWindow(args);
            }

            ImGui::PopStyleVar();

        }
        ImGui::End();

    }

    void ShowShuntDebugWindow(std::shared_ptr<Args> args)
    {
        ImGui::Begin("Debug");

        // Select Primary Shunt
        if (ImGui::RadioButton("Primary 75R", &args->rawData.PrimaryShunts, 0))
        {
            args->serialPort.AddCharToTxQueue('a');
        }
        if (ImGui::RadioButton("Primary 10R", &args->rawData.PrimaryShunts, 1))
        {
            args->serialPort.AddCharToTxQueue('s');
        }
        if (ImGui::RadioButton("Primary 1R", &args->rawData.PrimaryShunts, 2))
        {
            args->serialPort.AddCharToTxQueue('d');
        }
        if (ImGui::RadioButton("Primary Bypass", &args->rawData.PrimaryShunts, 3))
        {
            args->serialPort.AddCharToTxQueue('f');
        }
        ImGui::Separator();
        if (ImGui::RadioButton("Aux 75R", &args->rawData.AuxShunts, 0))
        {
            args->serialPort.AddCharToTxQueue('g');
        }
        if (ImGui::RadioButton("Aux 10R", &args->rawData.AuxShunts, 1))
        {
            args->serialPort.AddCharToTxQueue('h');
        }
        if (ImGui::RadioButton("Aux 1R", &args->rawData.AuxShunts, 2))
        {
            args->serialPort.AddCharToTxQueue('j');
        }
        if (ImGui::RadioButton("Aux Bypass", &args->rawData.AuxShunts, 3))
        {
            args->serialPort.AddCharToTxQueue('k');
        }

        ImGui::End();
    } // ShowShuntDebugWindow Method

    void ShowPlotWindow(std::shared_ptr<Args> args)
    {
        ImGui::Begin("Primary Channel", nullptr, ImGuiWindowFlags_NoCollapse);

        // ImGui::SliderFloat("History", &args->plotData.history, 1, 3, "%.1f s");

        ImGui::BeginDisabled();
        ImGui::RadioButton("Primary 75R", &args->rawData.PrimaryShunts, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Primary 10R", &args->rawData.PrimaryShunts, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Primary 1R", &args->rawData.PrimaryShunts, 2);
        ImGui::SameLine();
        ImGui::RadioButton("Primary Bypass", &args->rawData.PrimaryShunts, 3);
        ImGui::EndDisabled();

        if (ImPlot::BeginPlot("Primary Channel", ImVec2(-1, 600), ImPlotFlags_Crosshairs | ImPlotFlags_AntiAliased))
        {
            ImPlot::SetupAxes("ms", "Y-Axis 1", ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax, 0);
            ImPlot::SetupAxesLimits(0, args->plotData.history * 1000 - 1, 0, ImGuiCond_Always);

            ImPlot::SetupAxis(ImAxis_Y1, "Voltage", ImPlotAxisFlags_None);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 6);
            ImPlot::SetupAxisFormat(ImAxis_Y1, MetricFormatter, (void*)"V");

            ImPlot::SetupAxis(ImAxis_Y2, "Current", ImPlotAxisFlags_AuxDefault);
            ImPlot::SetupAxisLimits(ImAxis_Y2, 0, 0.001);
            ImPlot::SetupAxisFormat(ImAxis_Y2, MetricFormatter, (void*)"A");

            ImPlot::SetupAxis(ImAxis_Y3, "Power", ImPlotAxisFlags_AuxDefault);
            ImPlot::SetupAxisLimits(ImAxis_Y3, 0, 0.002);
            ImPlot::SetupAxisFormat(ImAxis_Y3, MetricFormatter, (void*)"W");

            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
            ImPlot::PlotLine("Voltage", args->plotData.primaryVoltage.data(), args->plotData.history * 1000, 1, 0, 0);

            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
            ImPlot::PlotLine("Current", args->plotData.primaryCurrent.data(), args->plotData.history * 1000, 1, 0, 0);

            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y3);
            ImPlot::PlotLine("Power", args->plotData.primaryPower.data(), args->plotData.history * 1000, 1, 0, 0);

            ImPlot::EndPlot();
        }

        ImGui::End();
    } // ShowPlotWindow Method

    bool CustomColorButton(const char* label, ImVec4 default, ImVec4 active, ImVec4 hover)
    {
        bool returnValue;
        ImGui::PushStyleColor(ImGuiCol_Button, default);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);

        returnValue = ImGui::Button(label);

        ImGui::PopStyleColor(3);

        return returnValue;
    }

    bool CustomColorButton(const char* label, ImVec2 size, ImVec4 default, ImVec4 active, ImVec4 hover)
    {
        bool returnValue;
        ImGui::PushStyleColor(ImGuiCol_Button, default);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);

        returnValue = ImGui::Button(label, size);

        ImGui::PopStyleColor(3);

        return returnValue;
    }
} // Namespace UserGui