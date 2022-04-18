#ifndef MENUDATA_H_
#define MENUDATA_H_

#include <thread>
#include <mutex>
#include "displayHandler.h"

struct Menu
{
    MenuOptions rootMenu;
    MenuOptions measureMenu;
    MenuOptions debugMenu;
    MenuOptions* activeMenu = &rootMenu;
    std::thread workerThread;
    int selectedIndex = 0;
    bool running = true;
    bool read = false;
    bool changed = false;

    double pollutionReading = 0;
    std::string coordinates = "";

    void Init();

    void JoinWorker();

    void Exit(std::mutex& m);

    void CursorUp(int index);

    void CursorDown(int index);

    void NextMenu(std::mutex& m);

    void Back(std::mutex& m);

    void ChangeMenu(MenuOptions* newMenu);

    virtual void StopReading(std::mutex& m);

    virtual void StartReading(std::mutex& m);

    virtual void GetReading(std::mutex& m);

    void Select(int index, std::mutex& m);

    void ResolveInstructionIndex(int instructionIndex, std::mutex& m);

    void DisplayUpdate(DisplayHandler& display, bool active);
};

#endif