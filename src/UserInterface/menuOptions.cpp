#include <array>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include "menuOptions.h"

void MenuOptions::SetButtonName(int index, std::string newName)
{
    buttons[index].name = newName;
};

void MenuOptions::SetButtonNavigationTarget(int index, MenuOptions* target)
{
    menuNavigationTarget[index] = target;
};