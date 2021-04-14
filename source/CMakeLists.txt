
# source and header files
set(
    headers
    "headers/base.hpp"
    "headers/accountant.hpp"
    "headers/errors.hpp"
    "headers/parser.hpp"
    "headers/app.hpp"
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