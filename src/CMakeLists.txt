cmake_minimum_required(VERSION 3.16)

project(hippodrome)		
# Setting the global variable that contains root path to the current project.
set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "include directory")
		

macro(add_source_folder folder_name)
  set(${folder_name}_files "")
  #message(STATUS ../${folder_name}/imp/*.cpp )
  file(GLOB_RECURSE ${folder_name}_files RELATIVE ${CMAKE_CURRENT_LIST_DIR}
												  ./${folder_name}/imp/*.cpp 
												  ./${folder_name}/imp/*.c
												  ./${folder_name}/*.h)
  source_group(${folder_name} FILES ${${folder_name}_files})
  #message(STATUS ${${folder_name}_files} ${${folder_name}_files})
  set(FILES_SOURCE ${FILES_SOURCE} ${${folder_name}_files})
endmacro()


add_source_folder(Authorization)
add_source_folder(DBManagment)
add_source_folder(Repositories)
add_source_folder(UserInteraction)
add_source_folder(Models)
add_source_folder(Types)

set(FILES_SOURCE ${FILES_SOURCE} main.cpp)		

#sqlite		
add_subdirectory(sqlite)
include_directories(${PROJECT_NAME} ${sqlite_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${FILES_SOURCE})
target_link_libraries(${PROJECT_NAME} sqlite_lib)
add_library(${PROJECT_NAME}_lib ${FILES_SOURCE})
target_link_libraries(${PROJECT_NAME} sqlite_lib)