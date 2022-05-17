#include "imgui.h"

namespace UserGui
{
	bool ShowConfigurationWindow(void* args)
	{
		ImGui::Begin("Configuration");
		ImGui::Text("Lets just see if this works here");
		ImGui::End();

		return true;
	}
} // Namespace UserGui