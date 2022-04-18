#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <ncurses.h>
#include "menuOptions.h"
#include "displayHandler.h"
#include "libsps30.h"
#include "neo6m.h"

struct DisplayHandler
{
    MenuOptions* options;
    int selected = 0;

    void Init()
    {
        //Placeholder function for all the interface initiation stuff
        //Get screen size
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);

        //Define the window for input
        WINDOW* menuwin = newwin(6, xMax-12, yMax-8, 5);
        box(menuwin, 0 ,0);
        refresh();
        wrefresh(menuwin);

        //Enable the arrow keys (woo)
        keypad(menuwin, true);
    };

    void Update()
    {
        for (int i = 0; i <= 5; i++)
        {
            if (i == selected)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i, 1, options->buttons[i].name);
            wattroff(menuwin, A_REVERSE);
        };
    };

    void UpdateMenuInactiveDisplay(int selected, MenuOptions* options, std::string lData, double pData)
    {
        std::cout << lData << " " << pData << std::endl;
    };

    void UpdateMenuDisplay(int selected, MenuOptions* options, std::string lData, double pData)
    {
        for (int i = 0; i <= 5; i++)
        {
            if (i == selected)
                std::cout << '*';
            std::cout << options->buttons[i].name << std::endl;
        };
    };
};