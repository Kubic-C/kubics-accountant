/* KA - app.h
what:
 * main app loop
*/

#pragma once
#ifndef KA_APP_H
#define KA_APP_H

#include "accountant.h"

namespace ka
{

    // prompt
    std::string prompt();

    // help commad
    void help();

    // call
    void call(cash_data& cd, std::string input);

    // main loop
    int run();
}

#endif // KA_APP_H