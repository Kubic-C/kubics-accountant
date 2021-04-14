#include "headers/accountant.h"
#include <time.h>
#include <fstream>
#include <queue>

namespace ka
{
    error set_doc(
        log& log,
        std::string args
    )
    {
        // test if "--doc" exists
        if(args.find("--doc") != std::string::npos)
        {
            return get_between(args, log.doc, '"', '"');
        }

        // add doc
        time_t now = time(0);
        tm* time_ = localtime(&now);

        log.doc = "on ("
            + std::to_string(time_->tm_mday) + '/'
            + std::to_string(time_->tm_mon+1) + '/'
            + std::to_string(time_->tm_year) + ") in the hour: "
            + std::to_string(time_->tm_hour);

        return no_error;
    }

    error make(
        cash_data* ctx, 
        std::string type,
        std::string sub_args
    )
    {
        if(type == "log")
            return make_log(ctx, sub_args);
        else if(type == "source")
            return make_source(ctx, sub_args);
        else  
            return specify_error_cause(uknown_type, type);
    }

    error make_source(
        cash_data* ctx,
        std::string args
    )
    {
        // get name and amount args
        std::string name, amount;
        check_find_str_get_next(args, "--name", name)
        check_find_str_get_next(args, "--amount", amount)

        // convert to an int
        int64_t amount_i = 0;
        if(!is_safe(attempt_stoi(amount, amount_i)))
            return specify_error_cause(invalid_argument, amount);

        // make the source
        ctx->sources[name].amount = amount_i;
        std::cout << "made new source\n";
        return no_error;
    }

    error make_log(
        cash_data* ctx,
        std::string args
    )
    {
        // arguments
        std::string id, from, to, amount;
        check_find_str_get_next(args, "--id", id)
        check_find_str_get_next(args, "--from", from)
        check_find_str_get_next(args, "--to", to)
        check_find_str_get_next(args, "--amount", amount)

        // convert into an int
        int64_t amount_i = 0;
        if(!is_safe(attempt_stoi(amount, amount_i)))
            return specify_error_cause(invalid_argument, amount);

        // is 'to' a valid source?
        if(ctx->sources.find(to) == ctx->sources.end())
            return specify_error_cause(invalid_argument, to + "(invalid source)");

        // make the log
        ctx->logs[id].amount = amount_i;
        ctx->logs[id].from = from;
        ctx->logs[id].to = to;

        // add doc
        error err = set_doc(ctx->logs[id], args);
        if(!is_safe(set_doc(ctx->logs[id], args)))
        {
            delete_data(ctx, id);
            return err;
        }

        // add log to list of logs in source
        // add or subtract the amount in source
        ctx->sources[to].logs.push_back(id);
        ctx->sources[to].amount += amount_i;

        std::cout << "made new log\n";
        return no_error; 
    }

    error delete_data(
        cash_data* ctx,
        std::string name
    )
    {
        source_map_iterator source_map_i = ctx->sources.find(name);
        if(source_map_i != ctx->sources.end())
            return delete_source(ctx, source_map_i);
        
        log_map_iterator log_map_i = ctx->logs.find(name);
        if(ctx->logs.find(name) != ctx->logs.end())
            return delete_log(ctx, log_map_i);
 
        return specify_error_cause(data_not_found,  name);
    }


    error delete_source(
        cash_data* ctx,
        source_map_iterator i
    )
    {
        // delete all logs in list of logs
        for(auto log : i->second.logs)
            ctx->logs.erase(log);

        ctx->sources.erase(i);
        std::cout << "source deleted\n";
        return no_error;
    }

    error delete_log(
        cash_data* ctx,
        log_map_iterator i
    )
    {
        if(ctx->sources.find(i->second.to) != ctx->sources.end())
        {
            // subtract money given/removed and remove log from list of logs in sources
            ctx->sources[i->second.to].amount -= i->second.amount;
            auto vec_i = std::find(ctx->sources[i->second.to].logs.begin(),
                      ctx->sources[i->second.to].logs.end(),
                      i->first);

            if(ctx->sources[i->second.to].logs.size() != 0)
                ctx->sources[i->second.to].logs.erase(vec_i);
        }

        ctx->logs.erase(i);
        std::cout << "log deleted\n";
        return no_error;
    }

    error cmd_make(
        cash_data* ctx, 
        std::string args
    )
    {
        error err =  make(ctx, get_first(args), get_rest(args));
        return err;
    }

    error cmd_delete(
        cash_data* ctx,
        std::string args
    )
    {
        return delete_data(ctx, get_first(args));
    }

    error show_data(
       cash_data* ctx,
       std::string name
   )
   {
        // special cases
        if(name == "all")
            return show_all(ctx);

        else if(name == "total")
            return show_total(ctx);

        // ordinary cases
        source_map_iterator source_map_i = ctx->sources.find(name);
        if(source_map_i != ctx->sources.end())
            return show_source(ctx, source_map_i);
        
        log_map_iterator log_map_i = ctx->logs.find(name);
        if(ctx->logs.find(name) != ctx->logs.end())
            return show_log(ctx, log_map_i);
 
        return specify_error_cause(data_not_found,  name);
   }


   error show_source(
       cash_data* ctx,
       source_map_iterator i
   )
   {
       std::cout << "source name: " << i->first << '\n';
       std::cout << "amount: " << i->second.amount << '\n';
       std::cout << "== relevant logs ==\n";
       for(auto str : i->second.logs)
       {
           show_log(ctx, ctx->logs.find(str));
           std::cout << "===================\n";
       }

       return no_error;
   }

    error show_log(
       cash_data* ctx,
       log_map_iterator i
    )
    {
        std::cout << "log id: " << i->first         << '\n';
        std::cout << "amount: " << i->second.amount << '\n';
        std::cout << "from: "   << i->second.from   << '\n';
        std::cout << "to: "     << i->second.to     << '\n';
        std::cout << "doc: "    << i->second.doc    << '\n';
        return no_error;
    }

    error show_total(
        cash_data* ctx
    )
    {
        int64_t total = 0;
        for(auto source : ctx->sources)
        {
            std::cout << "[=] ---- source ---- [=]\n";
            total += source.second.amount;
            show_source(ctx, ctx->sources.find(source.first));
            std::cout << "[=] ---------------- [=]\n";
        }

        std::cout << "TOTAL: " << total << '\n';

        return no_error;
   }

    error show_all(
        cash_data* ctx
    )
    {
        for(auto source : ctx->sources)
        {
            std::cout << "[=] ---- source ---- [=]\n";
            show_source(ctx, ctx->sources.find(source.first));
            std::cout << "[=] ---------------- [=]\n";
        }

        return no_error;
    }

    error cmd_show_data(
       cash_data* ctx,
       std::string args
    )
    {
        return show_data(ctx, get_first(args));
    }

    error load_ka_data(
        cash_data& cd,
        std::string dir
    )
    {
        std::ifstream file(dir);
        if(file.bad() || !file.is_open())
            return specify_error_cause(bad_file_dir, dir);

        std::string line = "";
        while(std::getline(file, line))
            print_err(cmd_make(&cd, line));

        std::cout << "file loaded successfully\n";
        file.close();
        return no_error;
    }

    error save_ka_data(
        cash_data& cd,
        std::string dir
    )
    {
        std::ofstream file(dir);
        if(file.bad() || !file.is_open())
            return specify_error_cause(bad_file_dir, dir);

        // since the way sources and logs
        // are put in a save file matter
        // we put string version of a log
        // and source into a vector
        // then combine into a bigger vector
        // to output to a file
        //
        // we have to do it this way becuase of:
        // 
        // cd.sources[log.second.to].amount -= 
        //        cd.logs[log.first].amount;

        // save logs
        std::vector<std::string> logs;
        for(auto log : cd.logs)
        {
            logs.push_back(
                  "log --id " + log.first
                + " --from " + log.second.from 
                + " --to " + log.second.to 
                + " --amount " + std::to_string(log.second.amount)
                + " --doc \"" + log.second.doc + "\""
            );

            cd.sources[log.second.to].amount -= 
                cd.logs[log.first].amount;
        }

        // save sources
        std::vector<std::string> sources;
        for(auto source : cd.sources)
        {
            sources.push_back(
                  "source --name " + source.first 
                + " --amount " + std::to_string(source.second.amount)
            );
        }

        // write logs and sources to the file
        std::vector<std::string> all;
        all.insert(all.begin(), sources.begin(), sources.end());
        all.insert(all.end(), logs.begin(), logs.end());
        for(auto ele : all)
            file << ele << '\n';

        file.close();
        load_ka_data(cd, dir);
        return no_error;
    }

    error cmd_load_ka_data(
        cash_data* cd,
        std::string args
    )
    {
        return load_ka_data(*cd, args);
    }

    error cmd_save_ka_data(
        cash_data* cd,
        std::string args
    )
    {
        return save_ka_data(*cd, args);
    }
}