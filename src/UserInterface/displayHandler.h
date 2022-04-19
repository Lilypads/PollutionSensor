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
    std::unordered_map<int, std::string> stateMap = {{0, "Acquiring GPS Fix"}, 
    {1, "Active"}, {2, "Acquiring Data"}, {3, "Cleanup"}};
    MenuOptions* options;
    int selected = 0;
    WINDOW* menuwin = NULL;
    SPS30measurement* displayPollution = NULL;
    neo6mMeasurment* displayGPS = NULL;
    std::string* stateName = nullptr;
    std::string* fileName = nullptr;

    void Init();

    void DisplayString(int y, int x, const char* string);

    void DisplaySPS30(int y, int x);

    void DisplayGPS(int y, int x);

    void MonitorMenuState(Menu* menu, int instruciton, DisplayHandler& display, std::mutex &m);

    void Update();

    void UpdateMenuInactiveDisplay(int selected, MenuOptions* options, std::string lData, double pData);

    void UpdateMenuDisplay(int selected, MenuOptions* options, std::string lData, double pData);
};

#endif // DISPLAYHANDLER_H_
