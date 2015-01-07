#include "cmd3.h"

int com_echo(std::string* args, std::ostream& printer)
{
    printer<<args[MAX_ARG_COUNT]; //The user pressing enter adds a newline for us
    return 0;
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
        current->next = NULL;
        return root;
    }
}
