#include "headers/parser.hpp"

namespace ka
{
    error find_str_get_next(
        std::string str,
        std::string search_str,
        std::string& return_str
    )
    {
        // find the starting index of the search_str in str
        size_t search_str_index = str.find(search_str);
        if(search_str_index == std::string::npos)
            return specify_error_cause(str_not_found, search_str);

        // get all characters after the search str
        std::string after_seach_str = 
            str.substr(search_str_index+search_str.size());

        // find the next white space after search str
        size_t first_white_space = after_seach_str.find(' ');
        if(first_white_space == std::string::npos)
        {
            return_str = after_seach_str;
            return no_error;
        }

        // then find the next white space after it
        size_t second_white_space = 
            (after_seach_str.substr(first_white_space+1)).find(' ');
        if(second_white_space == std::string::npos)
        {
            return_str = after_seach_str.substr(first_white_space+1);
            return no_error;
        }

        // the finally return the string between the two whitespaces
        return_str = after_seach_str.substr(first_white_space+1, 
                                   second_white_space);

        return no_error;
    }
    
    error attempt_stoi(
        std::string str,
        int64_t& int_
    )
    {
        try
        { 
            int_ = std::stoi(str);
        }
        catch(std::invalid_argument)
        { 
            return specify_error_cause(invalid_argument, str); 
        }

        return no_error;
    }

    std::string get_first(
        std::string str
    )
    {
        // get whitespace, if none return str arg
        size_t first_white_space = str.find(' ');
        if(first_white_space == std::string::npos)
            return str;

        // get set of characters
        return str.substr(0, first_white_space);
    }

    std::string get_rest(
        std::string str
    )
    {
        // get whitespace, if none return str arg
        size_t first_white_space = str.find(' ');
        if(first_white_space == std::string::npos)
            return str;

        // get rest of characters
        return str.substr(first_white_space+1);
    }

    error get_between(
        std::string str,
        std::string& return_str,
        char first,
        char second
    )
    {
        // get index of first
        size_t first_index = str.find(first);
        if(first_index == std::string::npos)
            return specify_error_cause(data_not_found, ""+first);

        // get index of second, substring incase first == second
        std::string rest = str.substr(first_index+1);
        size_t second_index = rest.find(second);
        if(second_index == std::string::npos)
            return specify_error_cause(data_not_found, ""+second);

        // substr
        return_str = rest.substr(0, second_index);
        return no_error;
    }
}