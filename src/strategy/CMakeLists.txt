add_library(strategy
    Strategy.cpp
    OrderedStrategy.cpp
    HuntingStrategy.cpp
    ParityStrategy.cpp
    ProbabilityStrategy.cpp)

target_link_libraries(strategy PRIVATE field)
target_include_directories(strategy PUBLIC ${PROJECT_SOURCE_DIR}/src)
