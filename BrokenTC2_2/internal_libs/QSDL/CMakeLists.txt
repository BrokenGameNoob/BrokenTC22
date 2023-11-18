cmake_minimum_required (VERSION 3.8)

set(RAW_LIB_NAME QSDL)
set(LIB_NAME lib${RAW_LIB_NAME})

############################
#
#       CMake config
#
############################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" "${CMAKE_ROOT}/Modules")


############################
#
#       Dependencies
#
############################

###############
#   SDL
###############
set(SDL2_DIR $ENV{SDL2_DIR} CACHE PATH "Path to SDL2")
list(APPEND CMAKE_PREFIX_PATH "${SDL2_DIR}")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
message("------- <SDL> -------")
message("SDL2_INCLUDE_DIRS=${SDL2_INCLUDE_DIRS}")
message("SDL2_LIBRARIES=${SDL2_LIBRARIES}")
message("----- END <SDL> -----")


###############
#   Qt
###############
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


############################
#
#       Project
#
############################

project(${LIB_NAME})


############################
#
#       Source files
#
############################

set(INCLUDE_DIR include/${RAW_LIB_NAME})
set(SRC_DIR src)
set(SOURCE_FILES
  ${INCLUDE_DIR}/game_controller.hpp ${SRC_DIR}/game_controller.cpp
  ${INCLUDE_DIR}/sdl_event_handler.hpp ${SRC_DIR}/sdl_event_handler.cpp
  ${INCLUDE_DIR}/sdl_global.hpp ${SRC_DIR}/sdl_global.cpp
)


############################
#
#       Targets
#
############################

# We declare the project as a static library and add all the source code files to it.
add_library(${LIB_NAME} STATIC ${SOURCE_FILES})
#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(${LIB_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})

target_compile_definitions(${LIB_NAME} INTERFACE CMAKE_COMPIL_MODE=${CMAKE_COMPIL_MODE_VAR})

target_include_directories(${LIB_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${LIB_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)
target_link_libraries(${LIB_NAME}
    PUBLIC
    libLogger::libLogger
    "${SDL2_LIBRARIES}"
    SDL2::SDL2
)

#Set target properties
target_include_directories(${LIB_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${SRC_DIR}
        ${SDL2_INCLUDE_DIRS}
)

target_compile_features(${LIB_NAME} PRIVATE cxx_auto_type)
target_compile_options(${LIB_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)
