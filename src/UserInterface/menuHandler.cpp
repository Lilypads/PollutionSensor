#include <string>
#include <array>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <signal.h>
#include <ncurses.h>
#include "menuOptions.h"
#include "displayHandler.h"
#include "inputHandler.h"
#include "menuHandler.h"

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

    void Init()
    {
        //Initialise ncurses
        initscr();
        noecho();
        cbreak();

        //Initialise the menu
        menu.Init();

        //Start input monitor and display thread
        inputThread = std::thread(&InputHandler::Listen, inputMonitor, std::ref(inputMonitor), std::ref(m));
        displayThread = std::thread(&MenuHandler::MonitorMenuState, this, std::ref(*this), std::ref(display), std::ref(m));
    };

    void MonitorMenuState(MenuHandler& handler, DisplayHandler& display, std::mutex &m)
    {
        Menu* menu = &handler.menu;
        while (menu->running)
        {
            Sleep(20);
            m.lock();
            if (menu->changed && handler.instruction == -1)
            {
                menu->DisplayUpdate(std::ref(display), handler.active);
                menu->changed = false;
            };
            m.unlock();
        };
    };

    void Start()
    {
        auto timeoutStart = std::chrono::high_resolution_clock::now();
        auto timeoutDuration = std::chrono::milliseconds(5000);
        auto now = std::chrono::high_resolution_clock::now();

        while(menu.running)
        {
            Sleep(20);
            instruction = inputMonitor.GetTopInput(std::ref(m));
            now = std::chrono::high_resolution_clock::now();
            if (instruction != -1)
            {
                timeoutStart = std::chrono::high_resolution_clock::now();
                active = true;
                m.lock();
                menu.changed = true;
                m.unlock();

                menu.ResolveInstructionIndex(instruction, std::ref(m));

                instruction = -1;
                continue;
            }
            else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timeoutStart) > timeoutDuration)
            {
                active = false;
                menu.DisplayUpdate(std::ref(display), active);
            };
        };

        Stop();
    };

    void Stop()
    {
        inputMonitor.listening = false;
        inputThread.join();
        displayThread.join();

        //End ncurses window display
        endwin();
    };

};