#ifndef DISPLAYHANDLER_H_
#define DISPLAYHANDLER_H_

#include <ncurses.h>
#include "menuOptions.h"

struct DisplayHandler
{
    MenuOptions* options;
    int selected = 0;
    WINDOW* menuwin = NULL;

    void Init();

    void Update();

    void UpdateMenuInactiveDisplay(int selected, MenuOptions* options, std::string lData, double pData);

    void UpdateMenuDisplay(int selected, MenuOptions* options, std::string lData, double pData);
};

#endif // DISPLAYHANDLER_H_
