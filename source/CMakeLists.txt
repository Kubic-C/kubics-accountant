
# source and header files
set(
    headers
    "headers/base.h"
    "headers/accountant.h"
    "headers/errors.h"
    "headers/parser.h"
    "headers/app.h"
)

set(
    sources
    "base.cpp"
    "accountant.cpp"
    "parser.cpp"
    "app.cpp"
)

# make an executable, and link any needed libraries
add_executable(
    kubics_accountant
    "main.cpp"
    ${headers}
    ${sources}
)