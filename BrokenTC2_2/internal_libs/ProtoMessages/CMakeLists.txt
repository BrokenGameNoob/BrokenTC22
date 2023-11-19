cmake_minimum_required (VERSION 3.8)
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_INSTALLED_DIR}/scripts/buildsystems/vcpkg.cmake")

set(RAW_LIB_NAME ProtoMessages)
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
#   Protobuf
###############

set(Protobuf_DIR "${VCPKG_INSTALLED_DIR}/x64-mingw-dynamic/share/protobuf")

if(EXISTS ${Protobuf_DIR})
message("PROTO DIR EXISTS")
else()
message("PROTO DIR NOT FOUND")
endif()

list(APPEND CMAKE_PREFIX_PATH "${Protobuf_DIR}")

message("Protobuf_DIR=${Protobuf_DIR}")

find_package(Protobuf CONFIG REQUIRED)

###############
#   Qt
###############
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Protobuf ProtobufTools REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#make shit works by forcing moc generation
cmake_policy(SET CMP0071 NEW)


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
  src/test.hpp
)

file(GLOB PROTO_FILES
  "proto/*.proto"
)
message("Proto files: ${PROTO_FILES}")

#qt_add_protobuf(internal_proto_lib
# PROTO_FILES
#   ${PROTO_FILES}
# QML
# QML_URI
#   btc2
# OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/internal_proto_lib"
#)
qt_add_protobuf(internal_proto_lib
  PROTO_FILES
    ${PROTO_FILES}
)

############################
#
#       Targets
#
############################

# We declare the project as a static library and add all the source code files to it.
add_library(${LIB_NAME} STATIC 
  ${SOURCE_FILES}
  # ${PROTO_SRCS}
  # ${PROTO_HDRS}
)

message("-- Proto headers")
message("${PROTO_HDRS}")
message("-- Proto sources")
message("${PROTO_SRCS}")

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(${LIB_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})

target_compile_definitions(${LIB_NAME} INTERFACE CMAKE_COMPIL_MODE=${CMAKE_COMPIL_MODE_VAR})

target_include_directories(${LIB_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${LIB_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)
target_link_libraries(${LIB_NAME}
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Protobuf
    libLogger::libLogger
    internal_proto_lib
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

message("----- END <${RAW_LIB_NAME}> -----")
