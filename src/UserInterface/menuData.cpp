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

    //Set internal funcitons for all buttons that have one
    measureMenu.buttons[5].function = std::bind<void(Menu::*)(), Menu*>(&Menu::Back, this);

    debugMenu.buttons[5].function = std::bind<void(Menu::*)(), Menu*>(&Menu::Back, this);

    rootMenu.buttons[0].function = std::bind<void(Menu::*)(), Menu*>(&Menu::NextMenu, this);
    rootMenu.buttons[1].function = std::bind<void(Menu::*)(), Menu*>(&Menu::NextMenu, this);
    rootMenu.buttons[5].function = std::bind<void(Menu::*)(), Menu*>(&Menu::Exit, this);

    //Set Button Names and/or Messages
    rootMenu.buttons[0].name = "Measure Menu";
    rootMenu.buttons[1].name = "Debug Menu";
    rootMenu.buttons[5].name = "Exit";

    measureMenu.buttons[0].name = "Start Measurement";
    measureMenu.buttons[1].name = "Stop Measurement";
    measureMenu.buttons[5].name = "Back";

    debugMenu.buttons[0].name = "There are currently no debug options";
    debugMenu.buttons[5].name = "Back";
};

void Menu::Exit()
{
    //Stop running
    running = false;
};

void Menu::CursorUp(int index)
{
    //Move the cursor up and make sure it's within bounds
    selectedIndex -= 1;
    if (selectedIndex < 0)
        selectedIndex = 5;
    //Continue moving the cursor up and keeping it within bounds until a button with a function is found
    while(activeMenu->buttons[selectedIndex].function == NULL)
    {
        selectedIndex -= 1;
        if (selectedIndex < 0)
            selectedIndex = 5;
    };
};

void Menu::CursorDown(int index)
{
    //Move the cursor down and make sure it's within bounds
    selectedIndex += 1;
    if (selectedIndex > 5)
        selectedIndex = 0;
    //Continue moving the cursor down and keeping it within bounds until a button with a function is found
    while(activeMenu->buttons[selectedIndex].function == NULL)
    {
        selectedIndex += 1;
        if (selectedIndex > 5)
            selectedIndex = 0;
    };
};

void Menu::NextMenu()
{
    //Go to the set of menu options that the selected button slot points to, if it points to another set of options
    MenuOptions* target = activeMenu->menuNavigationTarget[selectedIndex];
    if (target != nullptr)
        ChangeMenu(target);
};

void Menu::Back()
{
    //Go to the previous menu
    ChangeMenu(activeMenu->previousMenu);
};

void Menu::ChangeMenu(MenuOptions* newMenu)
{
    //Go to the given set of menu options and reset the selected item to index 0
    activeMenu = newMenu;
    selectedIndex = 0;

    //Make sure the selected item has a function and if it doesn't then move the selected index until it lands on a button with a function
    while(activeMenu->buttons[selectedIndex].function == NULL)
    {
        selectedIndex += 1;
        if (selectedIndex > 5)
            selectedIndex = 0;
    };
};

void Menu::Select(int index)
{
    //Perform the function the button points to
    Button* activeButton = &activeMenu->buttons[index];
    (activeButton->function)();
};

void Menu::ResolveInstructionIndex(int instructionIndex)
{
    //Maps the input codes to one of three actions
    if (instructionIndex == 0)
        Select(selectedIndex);
    else if (instructionIndex == 1)
        CursorUp(selectedIndex);
    else
        CursorDown(selectedIndex);
};