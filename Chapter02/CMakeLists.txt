cmake_minimum_required(VERSION 3.12)

project(Chapter02-Essential_C++_Techniques)

file(GLOB CHAPTER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")


find_library(MYLIB libmylibd.dylib HINTS ${CMAKE_BINARY_DIR}/mylib)
if(NOT MYLIB)
    message(FATAL_ERROR "mylib not found")
endif()


add_executable(${PROJECT_NAME} ${CHAPTER_SRC_FILES})
target_link_libraries(${PROJECT_NAME} GTest::gtest)
target_link_libraries(${PROJECT_NAME} ${MYLIB})