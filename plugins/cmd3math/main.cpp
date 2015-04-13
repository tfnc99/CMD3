#include "cmd3.h"
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <gmp.h>

/** com_sin, com_cos, com_tan
*       These the functions are exactly the same but with sin, cos, tan.
*       Pointers to them are passed in the struct sent to cmd3.
*/
int com_sin(std::string* args, std::ostream& printer)
{
    for(unsigned int i = 0; i < args[0].size(); i++)
        args[0][i] = tolower(args[0][i]);
    if(strncmp(args[0].c_str(),"pi\0",2) == 0)
    {
        printer<<sin(3.14159)<<std::endl;
        return 0;
    }
    printer<<sin(strtof(args[0].c_str(),NULL))<<std::endl;
    return 0;
}

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

int com_tan(std::string* args, std::ostream& printer)
{
    for(unsigned int i = 0; i < args[0].size(); i++)
        args[0][i] = tolower(args[0][i]);
    if(strncmp(args[0].c_str(),"pi\0",2) == 0)
    {
        printer<<tan(3.14159)<<std::endl;
        return 0;
    }
    printer<<tan(strtof(args[0].c_str(),NULL))<<std::endl;
    return 0;
}

com_base_conv(std::string* args, std::ostream& printer)
{
    int flag = 0;
    mpz_t number;
    mpz_init(number);
    mpz_set_ui(number,0);
    flag = mpz_set_str(number,args[0].c_str(),strtol(args[1].c_str(),))
}

extern "C"
{
    /** plugin_command_s* PLUGIN_SYMBOL_NAME3()
    *       Technically (preparing for version 4) the same plugin can exist for multiple
    *       versions of cmd, this is the first version that prepares for that. Also I don't
    *       think theres a way (use a template with a loop? but I guess thats a compile time
    *       thing) to do this without entering everything manually. So thats what going on here.
    */
    plugin_command_s* PLUGIN_SYMBOL_NAME3()
    {
        plugin_command_s* root = new plugin_command_s;
        plugin_command_s* current = root;
        current->command = "sin";
        current->description = "SIN [Number] :: Prints sine of number";
        current->to_call = &com_sin;
        current->next = new plugin_command_s;
        current = current->next;
        current->command = "cos";
        current->description = "COS [Number] :: Prints cosine of number";
        current->to_call = &com_cos;
        current->next = new plugin_command_s;
        current = current->next;
        current->command = "tan";
        current->description = "TAN [Number] :: Prints tangent of number";
        current->to_call = &com_tan;
        current->next = NULL;
        return root;
    }
}
