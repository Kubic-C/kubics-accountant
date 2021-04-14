#include "headers/app.h"
#include <filesystem>

namespace ka
{
    std::string prompt() 
    {
        std::cout << "\n[KA: " + std::filesystem::current_path().string() + "]>";
        std::string input;
        std::getline(std::cin, input);
        std::cout << '\n';
        return input;
    }

    void help()
    {
        std::cout << "load *path* : load a *.ka_data file\n";
        std::cout << "save *path* : save data by putting into file at *path*\n";
        std::cout << "make log --from ... --to ... --amount ... --id ... : make a new log or source\n";
        std::cout << "make source --name ... --amount ... : make a new source\n";
        std::cout << "delete *source or log name* : deletes a log or source by name\n";
        std::cout << "show *source or log name* : shows you all information regarding the log or source\n";
        std::cout << "show all : special case of the command \"show\", shows all sources\n";
        std::cout << "show total : special case of the command \"show\", shows total and all sources\n";
    }

    void call(cash_data& cd, std::string input)
    {
        std::string first = get_first(input);
        std::string rest = get_rest(input);

        if(first == "help")
            help();

        else if(first == "make")
            print_err(cmd_make(&cd, rest));

        else if(first == "delete")
            print_err(cmd_delete(&cd, rest));
        
        else if(first == "show")
            print_err(cmd_show_data(&cd, rest));

        else if(first == "save")
            print_err(cmd_save_ka_data(&cd, rest));

        else if(first == "load")
            print_err(cmd_load_ka_data(&cd, rest));

        else
            std::cout << "uknown command: " << first << '\n';
    }

    int run()
    {
        std::cout << "welcome to kuic's accoutant!\n";
        std::cout << "type help for more info\n";
        cash_data cd;

        while(true)
        {
            std::string input = prompt();
            if(input == "exit")
                break;
            else
                call(cd, input);
        }

        return 0;
    }
}