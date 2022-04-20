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
    //Set the default return value
    int returnVal = -1;
    m.lock();
    //Try to get an input from the input queue and if there are none there then return -1
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
        //Wait for 20ms to avoid using up the entire CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        //Tell ncurses to make reading functions non-blocking
        nodelay(menuwin, true);

        //Check for input
        input = wgetch(menuwin);

        //Check if the input is valid
        if (inputMap.find(input) != inputMap.end())
        {
            //Push the corrseponding input code to the handler's input queue
            m.lock();
            handler.inputs.push(inputMap[input]);
            m.unlock();
            input = -1;
        };
    };
};