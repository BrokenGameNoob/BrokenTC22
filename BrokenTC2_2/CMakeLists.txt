cmake_minimum_required(VERSION 3.16)

project(BrokenTC2_2 VERSION 0.1 LANGUAGES CXX)

add_compile_definitions(CMAKEMACRO_PROJECT_EXE="${CMAKE_PROJECT_NAME}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick QuickControls2 LinguistTools)

qt_standard_project_setup(REQUIRES 6.5)


############################
#
#       Source files
#
############################

###############
#   Sources
###############
set(BTC2_SOURCES
    main.cpp

    resources/resources.qrc
)

###############
#   QML Files
###############
set(BTC2_QML_FILES
    Main.qml
)

###############
#   Translations
###############
set(BTC2_TRANSLATIONS_FILES
    translations/${CMAKE_PROJECT_NAME}_fr.ts
)



############################
#
#       CMake commands
#
############################

qt_add_executable(${CMAKE_PROJECT_NAME}
    ${BTC2_SOURCES}
)

qt_add_qml_module(${CMAKE_PROJECT_NAME}
    URI ${CMAKE_PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${BTC2_QML_FILES}
)

qt_add_translations(${CMAKE_PROJECT_NAME} TS_FILES ${BTC2_TRANSLATIONS_FILES})
set_source_files_properties(${BTC2_TRANSLATIONS_FILES}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/translations")
add_dependencies(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lupdate)
add_dependencies(${CMAKE_PROJECT_NAME} update_translations)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${CMAKE_PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
