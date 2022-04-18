#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <ncurses.h>
#include "menuOptions.h"
#include "displayHandler.h"

struct DisplayHandler
{
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