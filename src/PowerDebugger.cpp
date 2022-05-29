#include <memory>
#include <thread>

#include "VulkanEngine.h"
#include "serial_comms.h"
#include "datatypes.h"
#include "serial_thread.h"

#ifdef WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main(int, char**)
{
    VulkanEngine vk;
    //std::shared_ptr<SerialPort> serialPort = std::make_shared<SerialPort>();

    std::shared_ptr<Args> args = std::make_shared<Args>();
    std::shared_ptr<bool> enableSerialLoop = std::make_shared<bool>(true);

    args->plotData.history = 3.0f;

    vk.Init();

    std::thread serialThread(SerialLoop, args, enableSerialLoop);

    vk.RunLoop(args);

    *enableSerialLoop = false;
    serialThread.join();

    vk.Cleanup();
    return 0;
}
