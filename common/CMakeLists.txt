# List of source files
set(SOURCES
    InputFile.cpp
    Config.cpp
    Runner.cpp
    TestHarness.cpp
)

add_library(Common ${SOURCES})
target_include_directories(Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Common PUBLIC GTest::gtest)