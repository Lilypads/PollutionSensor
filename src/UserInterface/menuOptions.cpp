#include <array>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include "menuOptions.h"

void MenuOptions::SetButtonName(int index, std::string newName)
{
    //Set the name of a given button
    buttons[index].name = newName;
};

void MenuOptions::SetButtonNavigationTarget(int index, MenuOptions* target)
{
    //Set the next menu target for a given button position
    menuNavigationTarget[index] = target;
};