/* KA - accountant.hpp
what:
 * accountant.hpp is where most of accountant magic happens :P
*/

#pragma once
#ifndef KA_ACCOUNTANT_HPP
#define KA_ACCOUNTANT_HPP

#include "parser.hpp"

namespace ka
{
    /* ka::log
    what:
     * a log is a log of some transaction
     * 
    */
    struct log
    {
        // from where the money came from  [from]
        std::string from;

        // where the money will be added   [to]
        std::string to;

        // amount of money recieved        [amount]
        int64_t amount;

        // data of creation
        std::string doc;
    };

    /* ka::source
    what:
     * a source is how much money you have in a certain
     * form or how much money you have in a certain place
     * so a source can be the amount of physical money
     * you have, how much digital money you have, etc.
     * 
    */
    struct source
    {
        // current amount of money
        int64_t amount;

        // all the logs that mention it
        std::vector<std::string> logs;
    };

    /* ka::cash_data
     * an instance of "cash_data" will hold
     * all data of a users logs and sources
    */
    struct cash_data
    {
        //       id/name      data
        std::map<std::string, source> sources;  
        std::map<std::string, log> logs;
    };

    // iterators used to access sources and logs
    typedef std::map<std::string, source>::iterator source_map_iterator;
    typedef std::map<std::string, log>::iterator log_map_iterator;

    /* error set_doc(log& log, std::string args);
    what:
     * set the date of creation of a log
    */
    error set_doc(
        log& log,
        std::string args
    );


    /* error make(cash_data* ctx, std::string type)
    what:
     * make will make a new type
     * of either a source or a log.
     * if it creates the type successfully,
     * it will store it in 'cash_data'
     * 
    params:
     * ctx: (context) if successful a new log 
     * or source will be added to to the cash_data/ctx
     * 
     * type: a string containing the name of the type
     * 
     * sub_args: everything after make <type> 
    */
    error make(
        cash_data* ctx, 
        std::string type,
        std::string sub_args
    );

    error make_source(
        cash_data* ctx,
        std::string args
    );

    error make_log(
        cash_data* ctx,
        std::string args
    );

    error cmd_make(
        cash_data* ctx, 
        std::string input
    );

    /* error delete_data(
     *      cash_data* ctx,
     *      std::string name
     * );
    what:cash_data* ctx, 
        std::string type,
     * will delete the data of
     * a log or source dependig
     * on the name/id.
     * 
     * the name arg can either be the name of
     * a source or an id of a log
     * 
    */
    error delete_data(
        cash_data* ctx,
        std::string name
    );

    error delete_source(
        cash_data* ctx,
        source_map_iterator i
    );

    error delete_log(
        cash_data* ctx,
        log_map_iterator i
    );

    error cmd_delete(
        cash_data* ctx,
        std::string args
    );

    /* error show_data(
     *      cash_data* ctx
     *      std::string name
     * );
    what:
     * prints data about a source or
     * log specified by name arg
    */
   error show_data(
       cash_data* ctx,
       std::string name
   );

   error show_source(
       cash_data* ctx,
       source_map_iterator i
   );

    error show_log(
       cash_data* ctx,
       log_map_iterator i
    );

    error show_total(
        cash_data* ctx
    );

    error show_all(
        cash_data* ctx
    );

    error cmd_show_data(
       cash_data* ctx,
       std::string args
    );

    /* file saving, and loading functions*/
    error load_ka_data(
        cash_data& cd,
        std::string dir
    );

    error save_ka_data(
        cash_data& cd,
        std::string dir
    );

    error cmd_load_ka_data(
        cash_data* cd,
        std::string args
    );

    error cmd_save_ka_data(
        cash_data* cd,
        std::string args
    );
}

#endif // KA_ACCOUNTANT_HPP