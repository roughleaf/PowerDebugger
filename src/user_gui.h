#pragma once
#include <memory>
#include "datatypes.h"
#include <GLFW/glfw3.h>

namespace UserGui
{
	void ShowUserWindow(GLFWwindow *window, std::shared_ptr<Args> args, std::shared_ptr<ConfigData> configData);
	void ShowConfigurationWindow(std::shared_ptr<Args> args, std::shared_ptr<ConfigData> configData);
	void ShowShuntDebugWindow(std::shared_ptr<Args> args);
	void ShowPlotWindow(std::shared_ptr<Args> args, std::shared_ptr<ConfigData> configData);

	bool CustomColorButton(const char* label, ImVec4 default, ImVec4 active, ImVec4 hover);
	bool CustomColorButton(const char* label, ImVec2 size, ImVec4 default, ImVec4 active, ImVec4 hover);
}// Namespace UserGui