add_executable(day-2 main.cpp)
target_link_libraries(day-2 GTest::gtest Common)

file(GLOB SOURCES "src/*.cpp")
target_sources(day-2 PRIVATE ${SOURCES})
target_include_directories(day-2 PRIVATE "src")


include(GoogleTest)
gtest_discover_tests(day-2)