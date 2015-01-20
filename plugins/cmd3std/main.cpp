#include <cstdlib>
#include <iostream>
#include <exception>
#include <fstream>
#include "cmd3.h"

/** Command: ECHO [Stuff]
*       Prints [Stuff] back into printer.
*/
int com_echo(std::string* args, std::ostream& printer)
{
    printer<<args[MAX_ARG_COUNT]; //The user pressing enter adds the space.
    return 0;
}

/** Command: CLEAR [None]
*       Clears the screen, because cygwin had issues with system() when it wasn't a
*       full installation I just decided to move this to a plugin compiled with MinGW.
*/
int com_clear(std::string* args, std::ostream& printer)
{
    return system("cls");
}

extern "C"
{
    plugin_command_s* PLUGIN_SYMBOL_NAME3()
    {
        plugin_command_s* root = new plugin_command_s;
        plugin_command_s* current = root;
        current->command = "echo";
        current->description = "ECHO [Stuff] :: Prints stuff";
        current->to_call = &com_echo;
        current->next = new plugin_command_s;
        current = current->next;
        current->command = "clear";
        current->description = "CLEAR [None] :: Clears the screen";
        current->to_call = &com_clear;
        current->next = NULL;
        return root;
    }
}
