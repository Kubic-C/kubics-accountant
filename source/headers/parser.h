/* KA - parser.h
what:
 * parses through strings to find specific substr
*/

#pragma once
#ifndef KA_PARSER_H
#define KA_PARSER_H

#include "errors.h"

#define check_find_str_get_next(str, search_str, return_str) \
            { \
                error search_error = \
                    ka::find_str_get_next(str, search_str, return_str); \
                \
                if(!ka::is_safe(search_error)) \
                    return search_error; \
            }

namespace ka
{
    /* std::string find_str_get_next(
     *      std::string str,
     *      std::string search_str,
     *      std::string& return_str
     *  );
    what:
     * find_str_get_next() will find a string
     * specified by the search_str arg and will get
     * everything after that is between two
     * white spaces.
     * 
     * if no string is found then it wiil
     * throw a "str_not_found_error"
     * 
     * if no white space is found it
     * will return everything AFTER
     * search_str
    example:
     * str = "some random string"
     * search_str = "some"
     * result = ""
     * err = find_str_get_next(str, search_str, result);
     * result == "random"
     * err.name == "no_error"
    */
    error find_str_get_next(
        std::string str,
        std::string search_str,
        std::string& return_str
    );


    /* error attempt_stoi(
     *  std::string str,
     *  int64_t& int_
     * );
    what:
     * similar to stoi
     * but when an invalid
     * argument is passed
     * instead returns an error
     * 
     * returns no_error if successful
     * invalid_argument otherwise
    */
    error attempt_stoi(
        std::string str,
        int64_t& int_
    );

    /* std::string get_first(
     *   std::string str
     * );
    what:
     * returns the first set of
     * characters in str arg before
     * a whitespace occurs
    */
    std::string get_first(
        std::string str
    );

    /* std::string get_rest(
     *   std::string str
     * );
    what:
     * gets all characters
     * after a white space
     * has occured
    */
    std::string get_rest(
        std::string str
    );

    /* error get_between(
     *      std::string str,
     *      std::string& return_str,
     *      char first,
     *      char second
     * )
    what:
     * gets a set of characters between two characters
    */
    error get_between(
        std::string str,
        std::string& return_str,
        char first,
        char second
    );
}

#endif // KA_PARSER_H