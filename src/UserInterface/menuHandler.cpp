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
    //Initialise ncurses to show the user interface
    initscr();
    noecho();
    cbreak();

    //Initialise the menu
    menu.Init();

    //Initialise the display and perform an initial update to show the first menu
    display.Init();
    display.UpdateCurrentMenu(activeMenu);
    display.Update();

    //Start input monitor and display threads to catch inputs and ensure new data from sensors can be displayed as it comes in
    displayThread = std::thread(&DisplayHandler::MonitorMenuState, display, &menu, std::ref(instruction), std::ref(display), std::ref(m));
    inputThread = std::thread(&InputHandler::Listen, inputMonitor, std::ref(display.menuwin), std::ref(inputMonitor), std::ref(m));
};

void MenuHandler::ForceDisplayUpdate()
{
    //Lock the changed variable and set it to true to make the display thread perform an update
    m.lock();
    menu.changed = true;
    m.unlock();
};

void MenuHandler::Start()
{
    //While the menu is still operating
    while(menu.running)
    {
        //Sleep for 20ms to prevent the main thread from using the whole CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        //Get the first instruction from the user
        instruction = inputMonitor.GetTopInput(std::ref(m));

        //If the instruction comes back as a valid code then process the input
        while (instruction != -1)
        {
            //Pass the instruction to the menu
            menu.ResolveInstructionIndex(instruction);

            //Update the selected index for the display
            display.UpdateSelectedIndex(menu.selectedIndex);

            //Reset the instruction to prevent accidentally repeating instructions
            instruction = inputMonitor.GetTopInput(std::ref(m));

            //Update the display to the new menu if it hasn't already been updated
            //if (display.options != menu.activeMenu)
            //    display.UpdateCurrentMenu(menu.activeMenu);

            //Force the display to update incase there was a change
            ForceDisplayUpdate();
            continue;
        }
    };

    //If we get here then the menu is no longer running so we can safely stop execution
    Stop();
};

void MenuHandler::Stop()
{
    //Tell the input monitor to stop monitoring input
    inputMonitor.listening = false;

    //Join the input and display threads
    inputThread.join();
    displayThread.join();

    //End ncurses window display
    endwin();
};