cmake_minimum_required(VERSION 3.23.2)

project(rpg-ng VERSION 0.1.0 DESCRIPTION "A small and simple 2D RPG engine")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# Set options
option(RPGNG_TEST "Enable unit tests" OFF)
option(RPGNG_DOCS "Enable compiling documentation" OFF)
option(BUILD_SHARED_LIBS "Build as a shared library" ON)
# To enable debug builds, use -DCMAKE_BUILD_TYPE=Debug

find_package(Lua REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
if(WIN32)
    find_package(jansson CONFIG REQUIRED)
else()
    find_package(Jansson REQUIRED)
endif()

add_executable(rpgng)
target_sources(rpgng
    PRIVATE
        "src/entity.c"
        "src/htable.c"
        "src/log.c"
        "src/main.c"
        "src/script.c"
        "src/window.c"
        "src/component/component.c"
        "src/component/dialogue.c"
        "src/component/inventory.c"
        "src/component/sprite.c"
        "src/component/transform.c"
)

target_compile_definitions(rpgng PUBLIC
    RPGNG_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    RPGNG_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    RPGNG_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    RPGNG_VERSION="${PROJECT_VERSION}"
)

# Set include directories
target_include_directories(rpgng SYSTEM PRIVATE ${LUA_INCLUDE_DIR})

# Set library version
#set_target_properties(rpgng PROPERTIES VERSION ${PROJECT_VERSION})

# Set link libraries
target_link_libraries(rpgng ${LUA_LIBRARIES})
target_link_libraries(rpgng SDL2::SDL2)
target_link_libraries(rpgng SDL2_image::SDL2_image)
target_link_libraries(rpgng jansson::jansson)

#if(WIN32)
#    target_link_libraries(rpgng cygwin1.dll)
#endif()

# Set compiler/language options
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS_RELEASE "-O2")
if(CYGWIN)
    set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
else()
    set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -pedantic-errors -fsanitize=address -fsanitize=undefined")
endif()

# If tests are enabled, make tests
#if(RPGNG_TEST)
#    enable_testing()
#
#    add_executable(map_tests test/map_tests.c test/Unity/src/unity.c)
#
#    target_link_libraries(map_tests rpgng jansson::jansson)
#
#    if(WIN32)
#        target_link_libraries(map_tests rpgng cygwin1.dll)
#    endif()
#
#    set_target_properties(map_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY test/bin)
#
#    add_test(NAME map_tests COMMAND test/bin/map_tests)
#
#    # Add library output dir to PATH, because in Windows, the loader will have
#    # no clue where to find anything
#    if(WIN32)
#        set_tests_properties(map_tests PROPERTIES ENVIRONMENT_MODIFICATION
#            PATH=path_list_append:${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
#    endif()
#endif()

# If documentation is enabled, compile docs
if(RPGNG_DOCS)
    find_package(Doxygen REQUIRED doxygen dot)

    add_custom_command(TARGET rpgng POST_BUILD COMMAND Doxygen::doxygen)
endif()
