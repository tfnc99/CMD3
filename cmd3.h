#ifndef PLUGINS_H_INCLUDED
#define PLUGINS_H_INCLUDED
#include <string>
#include <ostream>

/** Define's
*       These two are like defined rather than const so they can be used as funtion names.
*/
#define PLUGIN_SYMBOL_NAME3 plug3
#define PLUGIN_SYMBOL_NAME3_WITH_QUOTES "plug3"

/** static const things
*       These represent various buffer and size options, (and two quirky ones)
*       Make sure that if COMMAND_BUFFER_SIZE or ARG_BUFFER_SIZE are smaller than
*       INPUT_BUFFER_SIZE, otherwise its moot and justs wastes memory.
*       PLUGINS ARE NOT ALLOWED TO CHANGE THESE NUMBERS
*/
static const char CLEAR_SCREEN_COMMAND[] = "clear";
static const int INPUT_BUFFER_SIZE = 1023;
static const int COMMAND_BUFFER_SIZE = 64;
static const int MAX_ARG_COUNT = 8;
static const int ARG_BUFFER_SIZE = (INPUT_BUFFER_SIZE - COMMAND_BUFFER_SIZE) / MAX_ARG_COUNT;
static const char PROMPT_CHAR = '>';

/** struct command_s
*       This is what assemble returns, theres no point on it being in the API, but
*       this seemed like the best place to put it. Members should be self explanitory.
*/
struct command_s
{
    std::string command;
    std::string args[MAX_ARG_COUNT + 1];
};

/** struct plugin_command_s
*       This is what needes to be returned from the function returnedd from the plugin.
*       The linked list can be as long as needed but nothing must be empty and IT MUST
*       END WITH THE LAST NEXT BEING SET TO NULL. The members should be self-explanitory.
*       std::string command should be all lower case!
*/
struct plugin_command_s
{
    std::string command;
    std::string description;
    int(*to_call)(std::string*,std::ostream&);
    plugin_command_s* next;
};

/** Function pointer: plugin_ret_f
*       I wish you could use these as templetes for declared functions, anyway this is the
*       kind of function pointer that should be returned from the plugin (dlsym)
*/
typedef plugin_command_s* (*plugin_ret_f)();

#endif // PLUGINS_H_INCLUDED
