cmake_minimum_required (VERSION 3.0)
project (smart_dbg)

add_compile_options(-std=c++14)

include_directories(submodules/libelfin submodules/linenoise include)
add_executable(smart_dbg src/smart_dbg.cpp submodules/linenoise/linenoise.c)

add_executable(hello_world test_programs/hello_world.cpp)
set_target_properties(hello_world
                      PROPERTIES COMPILE_FLAGS "-g -O0")

add_executable(variable test_programs/variable.cpp)
set_target_properties(variable
                      PROPERTIES COMPILE_FLAGS "-gdwarf-2 -O0")

add_executable(unwinding test_programs/stack_unwinding.cpp)
set_target_properties(unwinding
                      PROPERTIES COMPILE_FLAGS "-g -O0")


add_custom_target(
   libelfin
   COMMAND make
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/submdoules/libelfin
)
target_link_libraries(smart_dbg
                      ${PROJECT_SOURCE_DIR}/submdoules/libelfin/dwarf/libdwarf++.so
                      ${PROJECT_SOURCE_DIR}/submdoules/libelfin/elf/libelf++.so)
add_dependencies(smart_dbg libelfin)