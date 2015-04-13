#ifndef PLUGINS_H_INCLUDED
#define PLUGINS_H_INCLUDED
#include <string>
#include <vector>
#include <ostream>

/** Define's
*       These two are like defined rather than const so they can be used as funtion names.
*/
#define PLUGIN_SYMBOL_NAME3 plug3
#define PLUGIN_SYMBOL_NAME3_WITH_QUOTES "plug3"

/** struct command_s
*       This is what assemble returns, theres no point on it being in the API, but
*       this seemed like the best place to put it. Members should be self explanitory.
*       The last element in args is just everything after the command.
*/
struct command_s
{
    std::string command;
    std::vector<std::string> args;
};

/** struct plugin_command_s
*       This is what needes to be returned from the function returned from the plugin.
*       The linked list can be as long as needed but nothing must be empty and IT MUST
*       END WITH THE LAST NEXT BEING SET TO NULL. The members should be self-explanitory.
*       std::string command should be all lower case!
*/
struct plugin_command_s
{
    std::string command;
    std::string description;
    int(*to_call)(std::vector<std::string>,std::ostream&);
    plugin_command_s* next;
};

/** Function pointer: plugin_ret_f
*       I wish you could use these as templetes for declared functions, anyway this is the
*       kind of function pointer that should be returned from the plugin (dlsym)
*/
typedef plugin_command_s* (*plugin_ret_f)();

#endif // PLUGINS_H_INCLUDED
