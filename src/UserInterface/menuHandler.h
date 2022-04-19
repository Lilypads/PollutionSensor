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

    bool active = false;        //There is no point in this given how updates work
    bool changed = false;
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
