#include <array>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include "menuOptions.h"

struct Button
{
    std::string name = "";
    std::function<void(std::mutex&)> function = NULL;
};

struct MenuOptions
{
    MenuOptions* previousMenu = nullptr;
    std::array<Button, 6> buttons;
    std::array<MenuOptions*, 6> menuNavigationTarget = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    void SetButtonName(int index, std::string newName)
    {
        buttons[index].name = newName;
    };

    void SetButtonNavigationTarget(int index, MenuOptions* target)
    {
        menuNavigationTarget[index] = target;
    };
};