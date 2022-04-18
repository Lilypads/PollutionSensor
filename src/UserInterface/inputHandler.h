#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

class InputHandler
{
private:
    std::unordered_map<int, int> inputMap = { {48, 0}, {49, 1}, {50, 2} };
    int input = -1;

public:
    bool listening = true;
    std::queue<int> inputs;

    int GetTopInput(std::mutex& m);

    void Listen(InputHandler& handler, std::mutex& m);
};

#endif // INPUTHANDLER_H_
