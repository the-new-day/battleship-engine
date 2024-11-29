add_executable(Battleship 
    main.cpp 
    Battleship.cpp)

add_subdirectory(field)
add_subdirectory(strategy)
add_subdirectory(api)

target_link_libraries(Battleship PRIVATE strategy field api)
