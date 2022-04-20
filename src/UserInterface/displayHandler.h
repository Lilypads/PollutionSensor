#ifndef DISPLAYHANDLER_H_
#define DISPLAYHANDLER_H_

#include <ncurses.h>
#include <string>
#include <unordered_map>
#include "libsps30.h"
#include "neo6m.h"
#include "menuOptions.h"
#include "menuData.h"

struct DisplayHandler
{
    //Define a pointer to the options in the menu currently open as well as the currently selected item,
    //the window to draw to, a pointer for pollution measurement data and a pointer for GPS measurement data
    //along with pointers to state and file name containers
    MenuOptions* options;
    int selected = 0;
    WINDOW* menuwin = NULL;
    SPS30measurement* displayPollution = NULL;
    neo6mMeasurment* displayGPS = NULL;
    std::string* stateName = nullptr;
    std::string* fileName = nullptr;

    void Init();

    void UpdateSelectedIndex(int newIndex);

    void UpdateCurrentMenu(MenuOptions* newOptions);

    void DisplayString(int y, int x, const char* string);

    void DisplaySPS30(int y, int x);

    void DisplayGPS(int y, int x);

    void MonitorMenuState(Menu* menu, int instruciton, DisplayHandler& display, std::mutex &m);

    void Update();

    void UpdateMenuInactiveDisplay(int selected, MenuOptions* options, std::string lData, double pData);

    void UpdateMenuDisplay(int selected, MenuOptions* options, std::string lData, double pData);
};

#endif // DISPLAYHANDLER_H_
