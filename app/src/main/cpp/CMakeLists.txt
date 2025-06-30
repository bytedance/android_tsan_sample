# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.22.1)

project("tsan_sample")

set(CMAKE_CXX_STANDARD 20)

add_library(tsan_sample SHARED native-lib.cpp)

target_compile_options(tsan_sample PUBLIC -fsanitize=thread)
set_target_properties(tsan_sample PROPERTIES LINK_FLAGS -fsanitize=thread)

find_library(log-lib log)
target_link_libraries(tsan_sample ${log-lib})

# TSan Loader
add_executable(tsan_loader tsan_loader.cpp)
target_link_libraries(tsan_loader ${log-lib})
set_target_properties(tsan_loader
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../../build/loader/resources/lib/arm64-v8a"
)
