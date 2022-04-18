#ifndef DISPLAYHANDLER_H_
#define DISPLAYHANDLER_H_

struct DisplayHandler
{
    void UpdateMenuInactiveDisplay(int selected, MenuOptions* options, std::string lData, double pData);

    void UpdateMenuDisplay(int selected, MenuOptions* options, std::string lData, double pData);
};

#endif // DISPLAYHANDLER_H_
