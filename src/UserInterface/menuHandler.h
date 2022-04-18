#ifndef MENUHANDLER_H_
#define MENUHANDLER_H_

class MenuHandler
{

private:

    Menu menu;
    MenuOptions *activeMenu = &menu.rootMenu;
    DisplayHandler display;
    int instruction = 0;

public:

    bool active = false;
    bool changed = false;
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
