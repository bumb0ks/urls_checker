cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)

project(service_one)

include(cmake/RecurseSubdirs.cmake)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)

set(PATH_TO_MYSQL "C:/Program Files/MySQL/MySQL Connector C++ 8.0")

set(BOOST_ROOT "C:/Users/bumbo/Documents/boost_1_69_0/")
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS         ON) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       OFF)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)

add_subdirectory( src )

find_package(Boost 1.69.0 COMPONENTS thread regex system date_time)
if(Boost_FOUND)
  add_compile_definitions(_WIN32_WINNT=0x0501) # 0x0A00

  include_directories(${Boost_INCLUDE_DIRS})

  include_directories(${PATH_TO_MYSQL}/include)
  link_directories(${PATH_TO_MYSQL}/lib64/vs14)

  set (MySql_LIBRARY ${PATH_TO_MYSQL}/lib64/vs14/mysqlcppconn.lib)
  
  add_executable(${PROJECT_NAME} ${SOURCES})
  target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${MySql_LIBRARY})

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/3rd_party"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

  endif()
