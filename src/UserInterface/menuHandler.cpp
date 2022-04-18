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
    displayThread = std::thread(&MenuHandler::MonitorMenuState, this, std::ref(*this), std::ref(display), std::ref(m));
    inputThread = std::thread(&InputHandler::Listen, inputMonitor, std::ref(display.menuwin), std::ref(inputMonitor), std::ref(m));
};

void MenuHandler::MonitorMenuState(MenuHandler& handler, DisplayHandler& display, std::mutex &m)
{
    Menu* menu = &handler.menu;
    while (menu->running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        m.lock();
        if (menu->changed && handler.instruction == -1)
        {
            menu->DisplayUpdate(std::ref(display), handler.active);
            menu->changed = false;
        };
        m.unlock();
    };
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
            m.lock();
            menu.changed = true;
            m.unlock();

            menu.ResolveInstructionIndex(instruction, std::ref(m));

            display.selected = menu.selectedIndex;

            instruction = -1;
            menu.DisplayUpdate(std::ref(display), active);
            continue;
        }
        else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timeoutStart) > timeoutDuration)
        {
            active = false;
            menu.DisplayUpdate(std::ref(display), active);
        };
        DisplayHandler disp = std::ref(display);
        menu.DisplayUpdate(disp, active);
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