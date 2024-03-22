cmake_minimum_required(VERSION 3.16)

project(sqlite)		
# Setting the global variable that contains root path to the current project.
set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "include directory")
#set(${PROJECT_NAME}_lib ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "include directory")
		


set(FILES_SOURCE sqlite3.h imp/sqlite3.c)	


add_library(${PROJECT_NAME}_lib ${FILES_SOURCE})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "Using GCC")
	find_package(Threads REQUIRED)
	message(STATUS ${CMAKE_THREAD_LIBS_INIT})#pthread library
	message(STATUS ${CMAKE_DL_LIBS})#dl libs
	target_link_libraries(${PROJECT_NAME}_lib Threads::Threads)
	target_link_libraries(${PROJECT_NAME}_lib ${CMAKE_DL_LIBS})
endif()