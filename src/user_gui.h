#pragma once
#include <memory>
#include "datatypes.h"

namespace UserGui
{
	void ShowConfigurationWindow(std::shared_ptr<Args> args, std::shared_ptr<ConfigData> canfigData);
	void ShowShuntDebugWindow(std::shared_ptr<Args> args);

	bool CustomColorButton(const char* label, ImVec4 default, ImVec4 active, ImVec4 hover);
	bool CustomColorButton(const char* label, ImVec2 size, ImVec4 default, ImVec4 active, ImVec4 hover);
}// Namespace UserGui