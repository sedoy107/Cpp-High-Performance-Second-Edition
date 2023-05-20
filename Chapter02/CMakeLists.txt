cmake_minimum_required(VERSION 3.12)

project(Chapter02-Essential_C++_Techniques)

file(GLOB CHAPTER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_library(MYLIB libmylib.so HINTS ${CMAKE_BINARY_DIR}/mylib)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_file(MYLIB mylib.dll HINTS ${CMAKE_BINARY_DIR}/mylib/${CMAKE_BUILD_TYPE})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Dawrin")
	find_library(MYLIB libmylibd.dylib HINTS ${CMAKE_BINARY_DIR}/mylib)
# ETC
endif()

if(NOT MYLIB)
    message(FATAL_ERROR "mylib not found")
endif()


add_executable(${PROJECT_NAME} ${CHAPTER_SRC_FILES})
target_link_libraries(${PROJECT_NAME} GTest::gtest)
target_link_libraries(${PROJECT_NAME} ${MYLIB})