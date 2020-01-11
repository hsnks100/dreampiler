set(CMAKE_BUILD_TYPE Debug)

find_package(BISON)
find_package(FLEX)

# bison_target(Parser parse.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
flex_target(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)



set (CMAKE_CXX_STANDARD 11)

# add_flex_bison_dependency(Scanner Parser)

include_directories(
    .
    )
    # ${CMAKE_CURRENT_SOURCE_DIR}
ADD_EXECUTABLE( app.out 
    # __scanner.cpp
    main.cpp
    kparser.cpp
    ${FLEX_Scanner_OUTPUTS}
    # ${BISON_Parser_OUTPUTS}
    )
