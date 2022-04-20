#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include <queue>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <ncurses.h>
#include <iostream>

//Define an input handler as containing a map for input codes from a keyboard to input codes for the program
//This allows multiple different types of input to easily be used if the program were to be expanded
class InputHandler
{
private:
    std::unordered_map<int, int> inputMap = { {10, 0}, {KEY_UP, 1}, {KEY_DOWN, 2} };
    int input = -1;

public:

    bool listening = true;

    std::queue<int> inputs;

    int GetTopInput(std::mutex& m);

    //Define a listening function to listen to inputs from a specific window
    void Listen(WINDOW* menuwin, InputHandler& handler, std::mutex& m);
};

#endif // INPUTHANDLER_H_
