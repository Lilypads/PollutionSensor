#ifndef MENUHANDLER_H_
#define MENUHANDLER_H_

#include <thread>
#include <mutex>
#include "menuData.h"
#include "inputHandler.h"

class MenuHandler
{
private:
    //Define the active menu as the root menu by default and instruction to no-instruction code
    MenuOptions *activeMenu = &menu.rootMenu;
    int instruction = -1;

public:
    //Define a handler as containing a display, menu, input thread, display thread, a way to get input through the input monitor, and a mutex for thread safety
    DisplayHandler display;
    Menu menu;
    std::thread inputThread;
    std::thread displayThread;
    InputHandler inputMonitor;
    std::mutex m;

    void Init();

    void ForceDisplayUpdate();

    void MonitorMenuState(Menu* menu, int instruciton, DisplayHandler& display, std::mutex &m);

    void Start();

    void Stop();

};

#endif // MENUHANDLER_H_