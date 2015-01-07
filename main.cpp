#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <getopt.h>
#include "cmd3.h"

/** command_s* assemble(char* input)
*       Takes the input in form of a C string and makes a nice formatted command_s out of it.
*   Arguments:
*       char* input
*           C string with a command and arguments seperated by spaces and newline and null terminated.
*   Notes:
*      *The command name and arguments are parsed seperately.
*      *This function uses a lot of the flags in cmd3.h if a bufffer or arg count isnt big enough then
*       go there rather than changing things in here.
*/
command_s* assemble(const char* input)
{
    std::string command;
    command.resize(COMMAND_BUFFER_SIZE);
    std::string args[MAX_ARG_COUNT + 1];
    command_s* returnie = new command_s;
    int input_iten = 0;
    int command_iten = 0;
    do
    {
        command[command_iten] = tolower(input[input_iten]);
        command_iten++;
        input_iten++;
    }
    while(input[input_iten] != '\n' && command_iten < COMMAND_BUFFER_SIZE && input_iten < INPUT_BUFFER_SIZE
            && !isspace(input[input_iten]));
    returnie->command = command;
    input_iten++;
    int arg_iten = 0;
    /** Weird variable: bool spacey
    *       This is used in an if statment after an argument is parsed to increment arg_iten.
    *       It prevents arg_iten from being incremented if multiple spaces are between the arguments.
    */
    bool spacey = false;
    args[MAX_ARG_COUNT] = input + input_iten;
    while(input[input_iten] != '\n' && input[input_iten] != '\0' && arg_iten < MAX_ARG_COUNT)
    {
        command_iten = 0;
        args[arg_iten].resize(ARG_BUFFER_SIZE);
        while(!isspace(input[input_iten]) && input[input_iten] != '\0')
        {
            spacey = false;
            args[arg_iten][command_iten] = input[input_iten];
            command_iten++;
            input_iten++;
        }
        if(isspace(input[input_iten]) && !spacey)
        {
            spacey = true;
            arg_iten++;
        }
        input_iten++;
    }
    for(int i = 0; i <= MAX_ARG_COUNT; i++)
        returnie->args[i] = args[i];
    return returnie;
}

/** int verify(plugin_command_s* to_verify)
*       Checks a linked list returned from plugin for validity.
*   Arguments:
*       plugin_command_s* to_verify
*           Linked list to actually check.
*   Notes:
*      *This is a last ditch effort to check that the plugin returned something edible.
*       there is no way to check to see if any pointers are valid so if theres a segfault,
*       it'll get triggered here.
*/
int verify(plugin_command_s* to_verify)
{
    plugin_command_s* current = to_verify;
    while(current != NULL)
    {
        if(current->command.empty())
            return 1;
        if(current->description.empty())
            return 1;
        if(current->to_call == NULL)
            return 1;
        current = current->next;
    }
    return 0;
}

/** Global: plugin_command_s* root
*       This is the linked list for all the plugged in commands. Its global because I'm lazy~
*/
plugin_command_s* root = NULL;

/** int attach(plugin_command_s* commands)
*       Attaches a linked list that was returned from a plugin to the internal one.
*   Arguments:
*       plugin_command_s* commands
*           Linked list from plugin.
*   Notes:
*      *Because it needs to find the link in the list BEFORE null it checks to see if root is
*       empty seperately.
*      *It accesses the global variable root twice.
*/
int attach(plugin_command_s* commands)
{
    if(root == NULL)
    {
        root = commands;
        return 0;
    }
    plugin_command_s* current = root;
    while(current->next != NULL)
    {
        current = current->next;
    }
    current->next = commands;
    return 0;
}

/** int execute(command_s* command)
*       Takes a pre-parsed command and runs it if avalible. Commands are explained in detail below.
*   Arguments:
*       command_s* command
*           Command parsed by assemble() (or by hand).
*   Notes:
*      *Built in commands are checked for before and seperate from plugin commands. If a plugin
*       tries to add a HELP or LOAD command, it will never be called because the built in ones
*       take precedence.
*      *It accesses the global variable 'root' for plugin commands twice.
*/
int execute(command_s* command)
{
    /** Command: HELP
    *       Prints a description of all avvalible commands. Prints the built in commands first
    *       the cycles through all the plugged in one.
    */
    if(strncmp(command->command.c_str(),"help",COMMAND_BUFFER_SIZE) == 0)
    {
        std::cout<<"HELP    [None]  :: Shows this useless dialog"<<std::endl;
        std::cout<<"CLEAR   [None]  :: Clears the screen"<<std::endl;
        std::cout<<"LOAD  [Filename]:: Loads dynamic commands"<<std::endl;
        std::cout<<"EXIT   [Value]  :: Exits command line"<<std::endl;
        plugin_command_s* current = root;
        while(current != NULL)
        {
            std::cout<<current->description<<std::endl;
            current = current->next;
        }
        return 0;
    }
    /** Command: CLEAR
    *       Clears the screen, just calls system(). Cluttered screens make me mad.
    */
    else if(strncmp(command->command.c_str(),"clear",COMMAND_BUFFER_SIZE) == 0)
    {
        system(CLEAR_SCREEN_COMMAND);
        return 0;
    }
    /** Command: LOAD [FILENAME]
    *       The meat of the program. I couldn't figure out win32's HINSTANC_HTAG HUMBLIDIUM
    *       nonscense so I just installed cygwin and used the POSIX (I think its posix) stuff.
    *       Its only like two functions so making a wrapper for windows shouldn't be too hard.
    *       Its all C so it shouldn't throw any exceptions but it casts the function pointer
    *       from dlsym() so it makes some assumptions (not that theres any other way to do it)
    *       so theres a tiny chance of a segfault.
    */
    else if(strncmp(command->command.c_str(),"load",COMMAND_BUFFER_SIZE) == 0)
    {
        void* handle = dlopen(command->args[0].c_str(),RTLD_LAZY);
        if(handle == NULL)
        {
            std::cout<<dlerror()<<std::endl;
            return 0;
        }
        plugin_ret_f to_connect = (plugin_ret_f) dlsym(handle,PLUGIN_SYMBOL_NAME3_WITH_QUOTES);
        if(to_connect == NULL)
        {
            std::cout<<dlerror()<<std::endl;
            return 0;
        }
        plugin_command_s* plugin_com = to_connect();
        if(verify(plugin_com))
        {
            std::cout<<"Invalid data returned from plugin"<<std::endl;
            return 0;
        }
        attach(plugin_com);
        return 0;
    }
    /** Command: EXIT
    *       If you need a description of what this does you either 1) Dont speak english very well,
    *       at which point keep going! Good job! and how are you reading these comments? or 2) You
    *       think this is a different programming language. Its C++ with a lot of 'legacy' C stuff
    *       mixed in. This exits the program with an optional exit code.
    */
    else if(strncmp(command->command.c_str(),"exit",COMMAND_BUFFER_SIZE) == 0)
    {
        exit(strtol(command->args[0].c_str(),NULL,0));
        return 0;
    }
    /** Subsection: Plugins
    *       This is the part that steps through all of pluged in commands. If none exist then it
    *       just doesn't do anything. If control gets to the end of the function (execute, I mean)
    *       then it prints the passive-aggressive 'Invalid command'.
    */
    plugin_command_s* current = root;
    while(current!=NULL)
    {
        if(strncmp(command->command.c_str(),current->command.c_str(),current->command.length()) == 0)
        {
            current->to_call(command->args,std::cout);
            return 0;
        }
        current = current->next;
    }
    std::cout<<"Invalid command"<<std::endl;
    return 0;
}

/** int main(int argc, char *argv[])
*       Main check for arguemtns before doing anything, -h exits right away.
*       Then it loops forever, for(;;) style because that looks more techy than while(true), reseting input to all \0
*       each time, using fgets to get a full line safely. The INPUT_BUFFER_SIZE const has one added to it each time
*       its used in main to make sure theres a null terminating character.
*       One the input's been gotten it is pummped through assemble, then execute, then deleteted to make room for the
*       next one.
*
*   Arguments should be self explanitory.
*   Notes:
*      *delete [] input and return 0 are never reached.
*/
int main(int argc, char *argv[])
{

    int c = 0;
    while((c = getopt (argc, argv, "he:")) != -1)
        switch (c)
        {
        case 'h':
            std::cout<<"Program Arguments:"<<std::endl;
            std::cout<<"-h: Prints this useless dialog"<<std::endl;
            std::cout<<"-e [Command]: Executes command before program start"<<std::endl;
            std::cout<<"Internal Commands:"<<std::endl;
            exit(execute(assemble("help\0")));
            break;
        case 'e':{
            command_s* to_exec = assemble(optarg);
            execute(to_exec);
            delete to_exec;
            break;}
        case '?':
            if (optopt == 'e')
                std::cerr<<"Option -"<<optopt<<" requires an argument."<<std::endl;
            else if (isprint (optopt))
                std::cerr<<"Invalid option `-"<<optopt<<"'."<<std::endl;
            else
                std::cerr<<"Invalid character `"<<std::hex<<optopt<<"'."<<std::endl;
            exit(0);
        default:
            abort();
        }
    /* Begin command line loop */
    char *input = new char[INPUT_BUFFER_SIZE + 1];
    std::cout<<"TPro CMD3\nType 'HELP' to see avalible commands"<<std::endl;
    for(;;)
    {
        for(int i = 0; i < INPUT_BUFFER_SIZE + 1; i++)
            input[i] = '\0';
        std::cout.put(PROMPT_CHAR);
        fgets(input, INPUT_BUFFER_SIZE + 1, stdin);      //Gets input and appends a null character
        if(input[0] == '\n')
            continue;
        command_s* command = assemble(input);
        execute(command);
        delete command;
    }
    delete [] input;
    return 0;
}
