cmake_minimum_required (VERSION 3.8)

set(RAW_LIB_NAME DataStructures)
set(LIB_NAME lib${RAW_LIB_NAME})

message("------- <${RAW_LIB_NAME}> -------")


############################
#
#       CMake config
#
############################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

############################
#
#       Dependencies
#
############################

###############
#   Qt
###############
#add_subdirectory(../Logger)


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

file(GLOB SOURCE_FILES
  "include/**/*.hpp"
  "**/*.cpp"
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


get_property(current_automoc_macro_names GLOBAL PROPERTY AUTOMOC_MACRO_NAMES)
list(APPEND current_automoc_macro_names
    "DS_DECLARE_STRUCT"
    "DS_DECLARE_MEMBER_DEBUG_STRING_FUNC"
    "DS_DECLARE_OSTREAM_OPERATOR"
    "DS_DECLARE_TO_JSON_FUNC"
    "DS_DECLARE_SAVE_AS_FUNC"
    "DS_DECLARE_SET_FROM_JSON_FUNC"
    "DS_DECLARE_FILL_FROM_FILE_FUNC"
)
set_property(GLOBAL PROPERTY AUTOMOC_MACRO_NAMES "${current_automoc_macro_names}")


target_compile_definitions(${LIB_NAME} INTERFACE CMAKE_COMPIL_MODE=${CMAKE_COMPIL_MODE_VAR})

target_include_directories(${LIB_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${LIB_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Qml
)
target_link_libraries(${LIB_NAME}
    PUBLIC
    libLogger::libLogger
    libUtils::libUtils
)

#Set target properties
target_include_directories(${LIB_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${SRC_DIR}
)

target_compile_features(${LIB_NAME} PRIVATE cxx_auto_type)
target_compile_options(${LIB_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

message("----- END <${RAW_LIB_NAME}> -----")
