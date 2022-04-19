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

    void Exit();

    void CursorUp(int index);

    void CursorDown(int index);

    void NextMenu();

    void Back();

    void ChangeMenu(MenuOptions* newMenu);

    //virtual void StopReading(std::mutex& m);

    //virtual void StartReading(std::mutex& m);

    //virtual void GetReading(std::mutex& m);

    void Select(int index);

    void ResolveInstructionIndex(int instructionIndex);

    void DisplayUpdate(DisplayHandler& display, bool active);
};

#endif