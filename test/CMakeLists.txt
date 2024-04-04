cmake_minimum_required(VERSION 3.16)

project(hippodrome-test)	

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Setting the variable that contains list of dependent directories.
set(DEPENDENT_INC_DIR ${CMAKE_CURRENT_LIST_DIR}
                      ${hippodrome_INCLUDE_DIR}
					  ${sqlite_INCLUDE_DIR})
# Setting the variable that contains the list of dependent targets.
set(DEPENDENT_TARGET hippodrome_lib sqlite_lib)
file(GLOB ${PROJECT_NAME}_SRC RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*.c" "*.cpp")
file(GLOB ${PROJECT_NAME}_INC RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*.h" "*.hpp")

enable_testing()


add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${${PROJECT_NAME}_INC})

target_include_directories(${PROJECT_NAME} PRIVATE ${DEPENDENT_INC_DIR})
target_link_libraries(${PROJECT_NAME} ${DEPENDENT_TARGET})
target_link_libraries(${PROJECT_NAME} gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
file(COPY test.db DESTINATION ${CMAKE_CURRENT_BINARY_DIR})