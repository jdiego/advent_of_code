add_library(Utils input_file.cpp string_operations.cpp vector_operations.cpp)
target_include_directories (Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
