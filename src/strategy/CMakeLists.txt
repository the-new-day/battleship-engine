add_library(strategy
    Strategy.cpp
    OrderedStrategy.cpp
    ProbabilityStrategy.cpp
    ParityStrategy.cpp)

target_link_libraries(strategy PRIVATE field)
target_include_directories(strategy PUBLIC ${PROJECT_SOURCE_DIR}/src)
