set(CMAKE_BUILD_TYPE Debug)

find_package(BISON)
find_package(FLEX)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

# bison_target(Parser parse.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
flex_target(Scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
# set(CMAKE_CXX_COMPILER /usr/bin/g++)


set (CMAKE_CXX_STANDARD 11)

# add_flex_bison_dependency(Scanner Parser)

include_directories(
    src
    )
    # ${CMAKE_CURRENT_SOURCE_DIR}
ADD_EXECUTABLE( app.out 
    # __scanner.cpp
    src/main.cpp
    src/kparser.cpp
    ${FLEX_Scanner_OUTPUTS}
    # ${BISON_Parser_OUTPUTS}
    )
