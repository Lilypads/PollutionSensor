#include <thread>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <ncurses.h>
#include <time.h>

#include <iostream>

#include "inputHandler.h"

int InputHandler::GetTopInput(std::mutex& m)
{
    int returnVal = -1;
    m.lock();
    if (!inputs.empty())
    {
        returnVal = inputs.front();
        inputs.pop();
    };
    m.unlock();
    return returnVal;
};

void InputHandler::Listen(WINDOW* menuwin, InputHandler& handler, std::mutex& m)
{
    while (handler.listening)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        nodelay(menuwin, true);
        input = wgetch(menuwin); //Need to get the menuwindow from somewhere
        //if (_kbhit())
        //    input = (int)_getch();
        if (inputMap.find(input) != inputMap.end())
        {
            m.lock();
            handler.inputs.push(inputMap[input]);
            m.unlock();
            input = -1;
        };
    };
};