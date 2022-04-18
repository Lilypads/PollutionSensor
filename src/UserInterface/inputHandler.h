#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include <queue>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <ncurses.h>
#include <iostream>

class InputHandler
{
private:
    std::unordered_map<int, int> inputMap = { {10, 0}, {KEY_UP, 1}, {KEY_DOWN, 2} };
    int input = -1;

public:
    bool listening = true;
    std::queue<int> inputs;

    int GetTopInput(std::mutex& m);

    void Listen(WINDOW* menuwin, InputHandler& handler, std::mutex& m);
};

#endif // INPUTHANDLER_H_
