add_executable(Battleship 
    main.cpp 
    Battleship.cpp 
    Command.cpp)

add_subdirectory(field)
add_subdirectory(strategy)

target_link_libraries(Battleship PRIVATE strategy field)
