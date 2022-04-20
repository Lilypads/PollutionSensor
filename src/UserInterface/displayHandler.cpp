#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <ncurses.h>
#include "menuOptions.h"
#include "displayHandler.h"
#include "libsps30.h"
#include "neo6m.h"



void DisplayHandler::Init()
{
    //Placeholder function for all the interface initiation stuff
    //Get the size of the screen
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    //Define the window for input
    menuwin = newwin(24, xMax-12, yMax-32, 5);

    //Draw a box to outline the interface
    box(menuwin, 0 ,0);

    //Refresh the display for all windows
    refresh();

    //Refresh the display window
    wrefresh(menuwin);

    //Enable the arrow keys
    keypad(menuwin, true);
};

void DisplayHandler::UpdateSelectedIndex(int index)
{
    //Set the selected index
    selected = index;
};

void DisplayHandler::UpdateCurrentMenu(MenuOptions* newOptions)
{
    //Set the options to display
    options = newOptions;
};

void DisplayHandler::MonitorMenuState(Menu* menu, int instruction, DisplayHandler& display, std::mutex& m)
{
    //While the menu is still running
    while (menu->running)
    {
        //Wait 20ms to avoid using too many resources
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        //Mutex lock so avoid mangling the data in the changed variable
        m.lock();

        //If the menu has changed
        if (menu->changed)
        {
            //Update the current menu for display
            if (display.options != menu->activeMenu)
                display.UpdateCurrentMenu(menu->activeMenu);

            //Perform a full display update
            display.Update();

            //Inform the menu that the display has been updated
            menu->changed = false;
        };
        //Unlock the mutex since we're done with sensitive variables
        m.unlock();
    };
};

void DisplayHandler::DisplayString(int y, int x, const char* string)
{
    //Display a string to menuwin without requiring the menuwin argument
    mvwprintw(menuwin, y, x, string);
};

void DisplayHandler::DisplaySPS30(int y, int x)
{
    //Get and display all the data from the SPS30 sensor in a column starting at a given y and x position
    DisplayString(y, x, ("Typical Particle Size: " + std::to_string(displayPollution->TypicalParcSize) + " um").c_str());
    DisplayString(y+1, x, ("Mass Concentraion PM1.0: " + std::to_string(displayPollution->MassConcPM1_0) + " ug/m^3").c_str());
    DisplayString(y+2, x, ("Mass Concentraion PM2.5: " + std::to_string(displayPollution->MassConcPM2_5) + " ug/m^3").c_str());
    DisplayString(y+3, x, ("Mass Concentraion PM4.0: " + std::to_string(displayPollution->MassConcPM4_0) + " ug/m^3").c_str());
    DisplayString(y+4, x, ("Mass Concentraion PM10.0: " + std::to_string(displayPollution->MassConcPM10_0) + " ug/m^3").c_str());
};

void DisplayHandler::DisplayGPS(int y, int x)
{
    //Get and display all the data from the GPS in a column starting at a given y and x position
    DisplayString(y, x, ("GPS Error: " + std::to_string(displayGPS->hdop*5) + " m").c_str());
    DisplayString(y+1, x, ("Altitude: " + std::to_string(displayGPS->alt_m) + " m").c_str());
    DisplayString(y+2, x, ("Latitude: " + std::to_string(displayGPS->latt_deg) + " Deg").c_str());
    DisplayString(y+3, x, ("Longitude: " + std::to_string(displayGPS->long_deg) + " Deg").c_str());
};

void DisplayHandler::Update()
{
    //Clear all the old data from the display to prevent overlaps and visual errors
    wclear(menuwin);

    //Draw a box to outline the interface
    box(menuwin, 0 ,0);

    ///Update data
    //If we have pollution data to display then dispaly it
    if (displayPollution != NULL)
        DisplaySPS30(8, 1);

    //If we have GPS data to display then display it
    if (displayGPS != NULL)
        DisplayGPS(15, 1);

    //If the state machine's current state is available then display it
    if (stateName != nullptr)
        DisplayString(1, 50, ("State: " + *stateName).c_str());
    if (fileName != nullptr && *stateName == "Acquiring Data")
        DisplayString(2, 50, ("Writing Data To " + *fileName).c_str());

    //Update button display to show any new buttons for a new menu and which is selected
    for (int i = 0; i <= 5; i++)
    {
        //If the button is the selected one then we can invert the colours
        if (i == selected)
            wattron(menuwin, A_REVERSE);

        //Display the buttons
        DisplayString(i+1, 1, options->buttons[i].name.c_str());

        //Un-invert the colours if they were inverted
        wattroff(menuwin, A_REVERSE);
    };

    //Refresh the menu to write new data to display
    wrefresh(menuwin);
};