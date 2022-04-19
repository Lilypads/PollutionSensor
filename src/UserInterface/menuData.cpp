#include <functional>
#include <mutex>
#include <thread>
#include "displayHandler.h"
#include "menuOptions.h"
#include "menuData.h"


void Menu::Init()
{
    //Set parents for child menus
    measureMenu.previousMenu = &rootMenu;
    debugMenu.previousMenu = &rootMenu;

    //Set children for parent menus
    rootMenu.menuNavigationTarget[0] = &measureMenu;
    rootMenu.menuNavigationTarget[1] = &debugMenu;

    //measureMenu.buttons[0].function = std::bind<void(Menu::*)(), Menu*>(&Menu::StartReading, this);
    //measureMenu.buttons[1].function = std::bind<void(Menu::*)(), Menu*>(&Menu::StopReading, this);
    measureMenu.buttons[5].function = std::bind<void(Menu::*)(), Menu*>(&Menu::Back, this);

    debugMenu.buttons[5].function = std::bind<void(Menu::*)(), Menu*>(&Menu::Back, this);

    rootMenu.buttons[0].function = std::bind<void(Menu::*)(), Menu*>(&Menu::NextMenu, this);
    rootMenu.buttons[1].function = std::bind<void(Menu::*)(), Menu*>(&Menu::NextMenu, this);
    rootMenu.buttons[5].function = std::bind<void(Menu::*)(), Menu*>(&Menu::Exit, this);

    //Set Button Values
    rootMenu.buttons[0].name = "Measure Menu";
    rootMenu.buttons[1].name = "Debug Menu";
    rootMenu.buttons[5].name = "Exit";

    measureMenu.buttons[0].name = "Start Measurement";
    measureMenu.buttons[1].name = "Stop Measurement";
    measureMenu.buttons[2].name = "Upload Last Measurement to Github";
    measureMenu.buttons[5].name = "Back";

    debugMenu.buttons[0].name = "There are currently no debug options";
    debugMenu.buttons[5].name = "Back";
};

void Menu::JoinWorker()
{
    if (workerThread.joinable())
        workerThread.join();
};

void Menu::Exit()
{
    running = false;
    JoinWorker();
};

void Menu::CursorUp(int index)
{
    selectedIndex -= 1;
    if (selectedIndex < 0)
        selectedIndex = 5;
    while(activeMenu->buttons[selectedIndex].function == NULL)
    {
        selectedIndex -= 1;
        if (selectedIndex < 0)
            selectedIndex = 5;
    };
};

void Menu::CursorDown(int index)
{
    selectedIndex += 1;
    if (selectedIndex > 5)
        selectedIndex = 0;
    while(activeMenu->buttons[selectedIndex].function == NULL)
    {
        selectedIndex += 1;
        if (selectedIndex > 5)
            selectedIndex = 0;
    };
};

void Menu::NextMenu()
{
    MenuOptions* target = activeMenu->menuNavigationTarget[selectedIndex];
    if (target != nullptr)
        ChangeMenu(target);
};

void Menu::Back()
{
    ChangeMenu(activeMenu->previousMenu);
};

void Menu::ChangeMenu(MenuOptions* newMenu)
{
    activeMenu = newMenu;
    selectedIndex = 0;
    while(activeMenu->buttons[selectedIndex].function == NULL)
    {
        selectedIndex += 1;
        if (selectedIndex > 5)
            selectedIndex = 0;
    };
};

/*
void Menu::StopReading(std::mutex& m)
{
    if (read)
    {
        read = false;
        m.lock();
        pollutionReading = -1;
        coordinates = "Done Reading";
        m.unlock();
        JoinWorker();
    };
};

void Menu::StartReading(std::mutex& m)
{
    if (!read)
    {
        read = true;
        workerThread = std::thread(&Menu::GetReading, this, std::ref(m));
    };
};


void Menu::GetReading(std::mutex& m)
{
    while (read)
    {
        //Dummy read function to simluate data coming in irregularly
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        m.lock();
        pollutionReading += 1;
        coordinates += ".";
        changed = true;
        m.unlock();
    };
};
*/

void Menu::Select(int index)
{
    Button* activeButton = &activeMenu->buttons[index];
    (activeButton->function)();
};

void Menu::ResolveInstructionIndex(int instructionIndex)
{
    if (instructionIndex == 0)
        Select(selectedIndex);
    else if (instructionIndex == 1)
        CursorUp(selectedIndex);
    else
        CursorDown(selectedIndex);
};
/*
void Menu::DisplayUpdate(DisplayHandler& display, bool active)
{
    display.options = activeMenu;
    display.Update();
    
    if (active == true)
    {
        display.UpdateMenuDisplay(selectedIndex, activeMenu, coordinates, pollutionReading);
        return;
    };
    display.UpdateMenuInactiveDisplay(selectedIndex, activeMenu, coordinates, pollutionReading);
    
};*/