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
#include "menuData.h"
#include "menuHandler.h"

void MenuHandler::Init()
{
    //Initialise ncurses
    initscr();
    noecho();
    cbreak();

    //Initialise the menu
    menu.Init();
    display.Init();
    display.options = activeMenu;
    display.Update();

    //Start input monitor and display thread
    displayThread = std::thread(&DisplayHandler::MonitorMenuState, display, &menu, std::ref(instruction), std::ref(display), std::ref(m));
    inputThread = std::thread(&InputHandler::Listen, inputMonitor, std::ref(display.menuwin), std::ref(inputMonitor), std::ref(m));
};

void MenuHandler::ForceDisplayUpdate()
{
    m.lock();
    menu.changed = true;
    m.unlock();
};

void MenuHandler::Start()
{
    auto timeoutStart = std::chrono::high_resolution_clock::now();
    auto timeoutDuration = std::chrono::milliseconds(5000);
    auto now = std::chrono::high_resolution_clock::now();

    while(menu.running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        instruction = inputMonitor.GetTopInput(std::ref(m));
        now = std::chrono::high_resolution_clock::now();
        if (instruction != -1)
        {
            timeoutStart = std::chrono::high_resolution_clock::now();
            active = true;

            menu.ResolveInstructionIndex(instruction);

            display.selected = menu.selectedIndex;

            instruction = -1;
            display.options = menu.activeMenu;
            ForceDisplayUpdate();
            //display.Update();
            //menu.DisplayUpdate(std::ref(display), active);
            continue;
        }
        //Timeout functionality commented out because it actually increases processing requirements
        /*else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timeoutStart) > timeoutDuration)
        {
            active = false;
            menu.DisplayUpdate(std::ref(display), active);
        };
        //DisplayHandler disp = std::ref(display);
        //menu.DisplayUpdate(disp, active);*/
    };

    Stop();
};

void MenuHandler::Stop()
{
    inputMonitor.listening = false;
    inputThread.join();
    displayThread.join();

    //End ncurses window display
    endwin();
};