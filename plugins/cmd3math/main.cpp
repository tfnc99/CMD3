#include "cmd3.h"
#include <cstring>
#include <cstdlib>
#include <cmath>

int com_cos(std::string* args, std::ostream& printer)
{
    for(unsigned int i = 0; i < args[0].size(); i++)
        args[0][i] = tolower(args[0][i]);
    if(strncmp(args[0].c_str(),"pi\0",2) == 0)
    {
        printer<<cos(3.14159)<<std::endl;
        return 0;
    }
    printer<<cos(strtof(args[0].c_str(),NULL))<<std::endl;
    return 0;
}

extern "C"
{
    plugin_command_s* PLUGIN_SYMBOL_NAME()
    {
        plugin_command_s* root = new plugin_command_s;
        plugin_command_s* current = root;
        current->command = "cos";
        current->description = "COS    [NUMBER] :: PRINTS COS OF NUMBER";
        current->to_call = &com_cos;
        current->next = NULL;
        return root;
    }
}
