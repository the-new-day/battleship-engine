add_executable(Battleship 
    main.cpp 
    Battleship.cpp
    ShipHandler.cpp)

add_subdirectory(field)
add_subdirectory(strategy)
add_subdirectory(api)

target_compile_options(Battleship PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(Battleship PRIVATE strategy field api)
