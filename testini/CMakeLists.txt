cmake_minimum_required(VERSION 3.5)

project(testini LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_SOURCES
        main.cpp
        iniworker.h
        iniworker.cpp
        test_iniworker.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL D:/googletest-1.13.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(testini ${PROJECT_SOURCES})
target_link_libraries(testini gtest_main)

install(TARGETS testini
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_custom_command(
  TARGET testini PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/test"
)

add_custom_command(
  TARGET testini POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/test"
                                      "${CMAKE_CURRENT_BINARY_DIR}/test"
)
