/* KA - errors.h
what:
 * defines errors used throughout KA
*/

#pragma once
#ifndef KA_ERRORS_H
#define KA_ERRORS_H

#include "base.h"

namespace ka
{
    // holds data about an error
    struct error
    {
        const std::string what;
        const std::string name;
    };

    const error uknown_type      = { "uknown type: ", "uknown_type"                  };
    const error str_not_found    = { "str not found: ", "str_not_found"              };
    const error invalid_argument = { "invalid argument: ", "invalid_argument"        };
    const error no_error         = { "no errors were caused", "no_error"             };
    const error data_not_found   = { "data not found: ", "data_not_found"            };
    const error bad_file_dir     = { "could not find file or file bad: ", "bad_file_dir" };
    const error char_wrong_pos   = { "characters in wrong pos: ", "char_wrong_pos"   };

    inline error specify_error_cause(error err, std::string cause)
    {
        std::string what = std::string(err.what) + cause;
        return { std::move(what), err.name };
    }

    // err == no error
    inline bool is_safe(error err)
    {
        return err.name == no_error.name;
    }

    inline void print_err(error err)
    {
        if(is_safe(err))
            return;

        std::cout << err.what << '\n';
    }
}

#endif // KA_ERRORS_H