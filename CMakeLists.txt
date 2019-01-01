cmake_minimum_required(VERSION 3.8)
project(FunctionalPatterns)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include/fp/collections.hpp
  ${PROJECT_SOURCE_DIR}/include/fp/patterns.hpp
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGET)
endif()

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_test
  ${PROJECT_SOURCE_DIR}/test/collectionsTest.cpp
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} GTest::GTest GTest::Main)

get_target_property(INSTALL_INCLUDES ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES)
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_test
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(DIRECTORY ${INSTALL_INCLUDES} DESTINATION .)

include(CTest)
add_test(NAME TestAll COMMAND ${PROJECT_NAME}_test)
