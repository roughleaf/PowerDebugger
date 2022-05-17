#include <memory>

#include "VulkanEngine.h"
#include "serial_comms.h"

int main(int, char**)
{
    VulkanEngine vk;
    std::shared_ptr<SerialPort> serialPort = std::make_shared<SerialPort>();

    vk.Init();
    vk.RunLoop(nullptr);
    vk.Cleanup();
    return 0;
}
