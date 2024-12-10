add_library(api ConsoleApi.cpp)
target_include_directories(api PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_options(api PRIVATE -Wall -Wextra -Wpedantic)
