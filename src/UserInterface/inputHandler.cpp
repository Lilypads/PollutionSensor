#include <thread>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <ncurses.h>
#include "inputHandler.h"

class InputHandler
{
private:
    std::unordered_map<int, int> inputMap = { {48, 0}, {49, 1}, {50, 2} };
    int input = -1;

public:
    bool listening = true;
    std::queue<int> inputs;

    int GetTopInput(std::mutex& m)
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

    void Listen(InputHandler& handler, std::mutex& m)
    {
        while (handler.listening)
        {
            Sleep(20);
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
};