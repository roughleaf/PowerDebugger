# CMakeList.txt : CMake project for PowerDebugger, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (${PROJECT_NAME}
	PowerDebugger.cpp
	VulkanEngine.cpp
	user_gui.cpp
	calculations.cpp
	serial_thread.cpp)

target_link_libraries(${PROJECT_NAME} imgui implot glfw)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)
target_link_libraries(${PROJECT_NAME} serialcomms)
#target_include_directories(PowerDebugger PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# TODO: Add tests and install targets if needed.
