add_executable(day-10 main.cpp)
target_link_libraries(day-10 GTest::gtest Common Eigen3::Eigen)

file(GLOB SOURCES "src/*.cpp")
target_sources(day-10 PRIVATE ${SOURCES})
target_include_directories(day-10 PRIVATE "src")

# Make Eigen headers available to the IDE
target_include_directories(day-10 PRIVATE ${EIGEN_INCLUDE_DIR})

include(GoogleTest)
gtest_discover_tests(day-10)