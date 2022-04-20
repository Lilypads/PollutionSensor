#ifndef MENUDATA_H_
#define MENUDATA_H_

#include <thread>
#include <mutex>
#include "displayHandler.h"

struct Menu
{
    //Define the menu as having a root, measure, and debug menu
    MenuOptions rootMenu;
    MenuOptions measureMenu;
    MenuOptions debugMenu;

    //Set the default menu to the root
    MenuOptions* activeMenu = &rootMenu;

    //Set the default selection to 0
    int selectedIndex = 0;

    //Set the menu to be running by default
    bool running = true;

    //Set the menu to be unchanged by default
    bool changed = false;

    void Init();

    void Exit();

    void CursorUp(int index);

    void CursorDown(int index);

    void NextMenu();

    void Back();

    void ChangeMenu(MenuOptions* newMenu);

    void Select(int index);

    void ResolveInstructionIndex(int instructionIndex);

};

#endif