#include <string>
#include <array>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <signal.h>
#include <ncurses.h>


#include "inputHandler.h"
#include "displayHandler.h"
#include "menuOptions.h"
#include "menuData.h"
#include "menuHandler.h"

class FunctionWrapper
{
public:
    WINDOW* window = NULL;

    void READ()
    {
        mvwprintw(window, 1, 50, "read :|");
        wrefresh(window);
    };
};

int main()
{
    MenuHandler handler;
    FunctionWrapper funcWrap;
    //CustomMenu customMenu;
    handler.Init();
    std::mutex blankMutex;
    // handler.menu.measureMenu.buttons[0].function = std::bind<void(FunctionWrapper::*)(std::mutex&), FunctionWrapper*>(&FunctionWrapper::READ, &funcWrap, std::placeholders::_1);
    handler.menu.measureMenu.buttons[0].function = std::bind(&FunctionWrapper::READ,funcWrap);
    funcWrap.window = handler.display.menuwin;
    handler.Start();
    return 0;
};
