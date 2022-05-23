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

template <typename T>
inline T RandomRange(T min, T max)
{
    T scale = rand() / (T)RAND_MAX;
    return min + scale * (max - min);
}

void Sparkline(const char* id, const float* values, int count, float min_v, float max_v, int offset, const ImVec4& col, const ImVec2& size)
{
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
    if (ImPlot::BeginPlot(id, size, ImPlotFlags_CanvasOnly | ImPlotFlags_NoChild)) {
        ImPlot::SetupAxes(0, 0, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
        ImPlot::SetupAxesLimits(0, count - 1, min_v, max_v, ImGuiCond_Always);
        ImPlot::PushStyleColor(ImPlotCol_Line, col);
        ImPlot::PlotLine(id, values, count, 1, 0, offset);
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::PlotShaded(id, values, count, 0, 1, 0, offset);
        ImPlot::PopStyleVar();
        ImPlot::PopStyleColor();
        ImPlot::EndPlot();
    }
    ImPlot::PopStyleVar();
}

namespace UserGui
{
    void ShowConfigurationWindow(std::shared_ptr<Args> args, std::shared_ptr<ConfigData> canfigData)
    {
        //static float history = 10.0f;

        ImGui::Begin("Plot Tests", nullptr, ImGuiWindowFlags_NoCollapse);

        ImGui::SliderFloat("History", &args->plotData.history, 1, 3, "%.1f s");

        static ImGuiTableFlags flags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
            ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;0;

        if (ImGui::BeginTable("##table", 3, flags, ImVec2(-1, 0))) {
            ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 75.0f);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 75.0f);
            ImGui::TableSetupColumn("Graph");
            ImGui::TableHeadersRow();
            ImPlot::PushColormap(ImPlotColormap_Cool);
            for (int row = 0; row < 6; row++)
            {
                ImGui::TableNextRow();
                static float data[300];
                static float datay[300];
                std::vector<float> datav;
                srand(row);
                for (int i = 0; i < 300; ++i)
                {
                    datav.push_back(RandomRange(0.0f, 10.0f));
                    data[i] = RandomRange(0.0f, 10.0f);
                    datay[i] = (float)i;
                }
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Primary V: %d", row);
                ImGui::TableSetColumnIndex(1);
                //ImGui::Text("%.3f V", data[offset]);
                ImGui::Text("%2.2fV", args->calculatedValues.PrimaryV);
                ImGui::TableSetColumnIndex(2);
                ImGui::PushID(row);
                
                switch (row)
                {
                case 0:
                {
                    float min_v = 0;
                    float max_v = 6.0f;
                    ImPlot::BeginPlot("Test Plot", ImVec2(-1, 250), ImPlotFlags_CanvasOnly | ImPlotFlags_NoChild);
                    ImPlot::SetupAxes(0, 0, 0, 0);
                    ImPlot::SetupAxesLimits(0, args->plotData.history * 1000 - 1, min_v, max_v, ImGuiCond_Always);
                    ImPlot::PlotLine("Primary Voltage", args->plotData.primaryVoltage.data(), args->plotData.history * 1000, 1, 0, 0);
                    ImPlot::EndPlot();
                }   break;
                case 1:
                {
                    float min_i = 0;
                    float max_i = 100.0f;
                    ImPlot::BeginPlot("Test Plot", ImVec2(-1, 100), ImPlotFlags_CanvasOnly | ImPlotFlags_NoChild);
                    ImPlot::SetupAxes("ms", "I", ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxesLimits(0, args->plotData.history * 1000 - 1, min_i, max_i, ImGuiCond_Always);
                    ImPlot::PlotLine("Primary Current", args->plotData.primaryCurrent.data(), args->plotData.history * 1000, 1, 0, 0);
                    ImPlot::EndPlot();
                }   break;
                case 2:
                {
                    float min_p = 0;
                    float max_p = 100.0f;
                    ImPlot::BeginPlot("Test Plot", ImVec2(-1, 100), ImPlotFlags_CanvasOnly | ImPlotFlags_NoChild);
                    ImPlot::SetupAxes("ms", "P", ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxesLimits(0, args->plotData.history * 1000 - 1, min_p, max_p, ImGuiCond_Always);
                    ImPlot::PlotLine("Primary Power", args->plotData.primaryPower.data(), args->plotData.history * 1000, 1, 0, 0);
                    ImPlot::EndPlot();
                }   break;
                }
                    

                
                ImGui::PopID();
            }
            ImPlot::PopColormap();
            ImGui::EndTable();
            ImGui::End();

            ImVec2 btnSize{ 100,20 };
            ImVec4 btnGreenOn{ 0, 0.65f, 0, 1 };
            ImVec4 btnGreenOnActive{ 0, 0.45f, 0, 1 };
            ImVec4 btnGreenOnHover{ 0, 0.55f, 0, 1 };

            ImVec4 btnRedOff{ 0.3f, 0, 0, 1 };
            ImVec4 btnRedOffActive{ 0.2f, 0, 0, 1 };
            ImVec4 btnRedOffHover{ 0.15f, 0, 0, 1 };       
            

            ImGui::Begin("Controls");
            ImGui::BeginDisabled(canfigData->portIsOpen);
            ImGui::SetNextItemWidth(150);
            ImGui::Combo(" ", &canfigData->selectedPort, canfigData->comboPortItems, canfigData->serialPortList.size());
            ImGui::EndDisabled();
            //ImGui::SameLine();
            if (!canfigData->portIsOpen)
            {
                if (ImGui::Button("Open Port", ImVec2(150, 20)))
                {
                    if (args->serialPort.OpenPort(canfigData->serialPortListFull[canfigData->selectedPort].c_str()))
                    {
                        canfigData->portIsOpen = true;
                    }
                }
            }
            else
            {
                if (ImGui::Button("Close Port", ImVec2(150, 20)))
                {
                    if (args->serialPort.ClosePort())
                    {
                        canfigData->portIsOpen = false;
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
	}

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