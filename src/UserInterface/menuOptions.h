#ifndef MENUOPTIONS_H_
#define MENUOPTIONS_H_

#include <array>
#include <string>
#include <functional>
#include <ncurses.h>

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

    void SetButtonName(int index, std::string newName);

    void SetButtonNavigationTarget(int index, MenuOptions* target);
};

#endif // MENUOPTIONS_H_