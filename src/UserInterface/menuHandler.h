#ifndef MENUHANDLER_H_
#define MENUHANDLER_H_

#include <thread>
#include <mutex>
#include "menuData.h"
#include "inputHandler.h"

class MenuHandler
{

private:

    MenuOptions *activeMenu = &menu.rootMenu;
    int instruction = 0;

public:

    bool active = false;
    bool changed = false;
    DisplayHandler display;
    Menu menu;
    std::thread inputThread;
    std::thread displayThread;
    InputHandler inputMonitor;
    std::mutex m;

    void Init();

    void MonitorMenuState(MenuHandler& handler, DisplayHandler& display, std::mutex &m);

    void Start();

    void Stop();

};

#endif // MENUHANDLER_H_
