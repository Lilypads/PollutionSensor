#ifndef MENUOPTIONS_H_
#define MENUOPTIONS_H_

#include <array>
#include <string>
#include <functional>
#include <ncurses.h>

//Define a button as having a name and a function call
struct Button
{
    std::string name = "";
    std::function<void()> function = NULL;
};

//Define a set of menu options as a pointer to the last menu, a set of buttons, and a set of other options
//that each button can take the user to
struct MenuOptions
{
    MenuOptions* previousMenu = nullptr;
    std::array<Button, 6> buttons;
    std::array<MenuOptions*, 6> menuNavigationTarget = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    void SetButtonName(int index, std::string newName);

    void SetButtonNavigationTarget(int index, MenuOptions* target);
};

#endif // MENUOPTIONS_H_