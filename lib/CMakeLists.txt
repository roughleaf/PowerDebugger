find_package(Vulkan REQUIRED)

add_library(imgui STATIC)
target_include_directories(imgui PUBLIC imgui)
target_sources(imgui PRIVATE
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_impl_vulkan.cpp
    imgui/imgui_impl_glfw.cpp
)

add_library(implot STATIC)
target_include_directories(implot PUBLIC implot)
target_sources(implot PRIVATE
    implot/implot.cpp
    implot/implot_demo.cpp
    implot/implot_items.cpp
)

add_library(serialcomms STATIC)
target_include_directories(serialcomms PUBLIC serialcomms)
target_sources(serialcomms PRIVATE
    serialcomms/serial_comms.cpp)

target_link_libraries(implot PUBLIC imgui)
target_link_libraries(imgui PUBLIC Vulkan::Vulkan glfw)

