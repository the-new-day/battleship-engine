add_library(field 
    Field.cpp
    MatrixField.cpp
    CompressedField.cpp
    CompressedDenseField.cpp
    MappedField.cpp
    RleBlock.cpp)

target_compile_options(field PRIVATE -Wall -Wextra -Wpedantic)
