cmake_minimum_required(VERSION 3.16)

project(BrokenTC2_2 VERSION 0.1 LANGUAGES CXX)

add_compile_definitions(CMAKEMACRO_PROJECT_EXE="${CMAKE_PROJECT_NAME}")
add_compile_definitions(CMAKEMACRO_BASE_PROJECT_DIR="${CMAKE_SOURCE_DIR}")

set(QT_VERSION_MAJOR 6 CACHE INTERNAL "")
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set(VCPKG_INSTALLED_DIR "${PARENT_DIR}/vcpkg_installed" CACHE INTERNAL "")
message("VCPKG_INSTALLED_DIR=${VCPKG_INSTALLED_DIR}")


############################
#
#       CMake config
#
############################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" "${CMAKE_ROOT}/Modules")

############################
#
#            Qt
#
############################

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Widgets Quick QuickControls2 Xml LinguistTools Concurrent)

qt_standard_project_setup(
  I18N_TRANSLATED_LANGUAGES fr
)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 OLD)
cmake_policy(SET CMP0071 NEW)

############################
#
#       Target creation
#
############################

qt_add_executable(${CMAKE_PROJECT_NAME}
    ${BTC2_SOURCES}
)


############################
#
#       Dependencies
#
############################

Include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG "v1.12.0"
)
FetchContent_MakeAvailable(spdlog)

#set(SDL2_PATH "C:\\Libs\\x86_64-w64-mingw32\\" CACHE BOOL "" FORCE)
#set(SDL2_INCLUDE_DIR "C:\\Libs\\x86_64-w64-mingw32\\include\\" CACHE BOOL "" FORCE)
#include_directories(${SDL2_INCLUDE_DIR})
#find_package(SDL2 REQUIRED)

#add_subdirectory(internal_libs/GitVersion)
#add_subdirectory(internal_libs/Logger)
#add_subdirectory(internal_libs/QSDL)
add_subdirectory(internal_libs)


############################
#
#         Options
#
############################

message("CMAKE_COMPIL_MODE_VAR=${CMAKE_COMPIL_MODE_VAR}")
if(CMAKE_COMPIL_MODE_VAR EQUAL 0)
    set(CUSTOM_BUILD_TRANSLATIONS OFF)
else()
    set(CUSTOM_BUILD_TRANSLATIONS ON)
endif()


############################
#
#       Source files
#
############################

###############
#   Resources
###############
set(BTC2_SOURCES_DIR assets)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/${BTC2_SOURCES_DIR}/app_icon_resource.rc")
set(BTC2_RESOURCES
    ${APP_ICON_RESOURCE_WINDOWS}
    ${BTC2_SOURCES_DIR}/resources.qrc
)

###############
#   Sources
###############
set(BTC2_SOURCES_DIR src)
set(BTC2_SOURCES
    ${BTC2_SOURCES_DIR}/main.cpp

    # Debug
#    ${BTC2_SOURCES_DIR}/debug/qstring_callback_sink.hpp
    ${BTC2_SOURCES_DIR}/debug/qml_log.hpp ${BTC2_SOURCES_DIR}/debug/qml_log.cpp

    # Utils
    ${BTC2_SOURCES_DIR}/utils/qt_utils.hpp
    ${BTC2_SOURCES_DIR}/utils/style.hpp
    ${BTC2_SOURCES_DIR}/utils/shared_constants.hpp
    ${BTC2_SOURCES_DIR}/utils/time.hpp

    # Services
    ${BTC2_SOURCES_DIR}/system/services/service_manager.hpp ${BTC2_SOURCES_DIR}/system/services/service_manager.cpp
    ${BTC2_SOURCES_DIR}/system/services/controller_handler.hpp ${BTC2_SOURCES_DIR}/system/services/controller_handler.cpp
    ${BTC2_SOURCES_DIR}/system/services/keyboard_handler.hpp ${BTC2_SOURCES_DIR}/system/services/keyboard_handler.cpp
    ${BTC2_SOURCES_DIR}/system/services/model_registry.hpp ${BTC2_SOURCES_DIR}/system/services/model_registry.cpp
    ${BTC2_SOURCES_DIR}/system/services/game_profiles_handler.hpp ${BTC2_SOURCES_DIR}/system/services/game_profiles_handler.cpp
    ${BTC2_SOURCES_DIR}/system/services/screen_overlay_selector.hpp ${BTC2_SOURCES_DIR}/system/services/screen_overlay_selector.cpp

    # Controls
    ${BTC2_SOURCES_DIR}/system/controls_io/keystroke_sequencer.hpp ${BTC2_SOURCES_DIR}/system/controls_io/keystroke_sequencer.cpp

    # Games
    ${BTC2_SOURCES_DIR}/games/gear_handler_factory.hpp ${BTC2_SOURCES_DIR}/games/gear_handler_factory.cpp
    ${BTC2_SOURCES_DIR}/games/gear_handler.hpp ${BTC2_SOURCES_DIR}/games/gear_handler.cpp
    ${BTC2_SOURCES_DIR}/games/gear_handler_the_crew.hpp ${BTC2_SOURCES_DIR}/games/gear_handler_the_crew.cpp
    ${BTC2_SOURCES_DIR}/games/easy_setup_interface.hpp ${BTC2_SOURCES_DIR}/games/easy_setup_interface.cpp
    ${BTC2_SOURCES_DIR}/games/config_editor.hpp ${BTC2_SOURCES_DIR}/games/config_editor.cpp

    ${BTC2_RESOURCES}
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${BTC2_SOURCES_DIR})

###############
#   QML Files
###############
set(BTC2_QML_DIR src)
set(BTC2_QML_FILES
    ${BTC2_QML_DIR}/Main.qml
    ${BTC2_QML_DIR}/SoftStatusBar.qml

    # Utils
    ${BTC2_QML_DIR}/utils/QMLStyle.qml
    ${BTC2_QML_DIR}/utils/Utils.js

    # Widgets
    ${BTC2_SOURCES_DIR}/widgets/ScrollableTextArea.qml
    ${BTC2_SOURCES_DIR}/widgets/LabelledIcon.qml
    ${BTC2_SOURCES_DIR}/widgets/GroupedEditor.qml
    ${BTC2_SOURCES_DIR}/widgets/ColoredImage.qml
    ${BTC2_SOURCES_DIR}/widgets/ThemedPopup.qml
    ${BTC2_SOURCES_DIR}/widgets/LoadingIcon.qml
    ${BTC2_SOURCES_DIR}/widgets/ConfirmationDialog.qml
    ${BTC2_SOURCES_DIR}/widgets/GameOverlay/GameOverlay.qml
    ${BTC2_SOURCES_DIR}/widgets/GameOverlay/EnableDragForComponent.qml
    ${BTC2_SOURCES_DIR}/widgets/GameOverlay/GameOverlayEditMenu.qml
    ${BTC2_SOURCES_DIR}/widgets/GameOverlay/SoftRunningIcon.qml
    ${BTC2_SOURCES_DIR}/widgets/ColorPicker.qml
    ${BTC2_SOURCES_DIR}/widgets/ButtonOk.qml
    ${BTC2_SOURCES_DIR}/widgets/ButtonCancel.qml
    ${BTC2_SOURCES_DIR}/widgets/LoadingLabel.qml

    # Tabs
    ${BTC2_QML_DIR}/tabs/SettingsTab.qml
    ${BTC2_QML_DIR}/tabs/SoftwareControlsTab.qml
    ${BTC2_QML_DIR}/tabs/GameProfileTab.qml

    # Panels
    ${BTC2_QML_DIR}/panels/MainPanel.qml
    ${BTC2_QML_DIR}/panels/ControllerListPanel.qml
    ${BTC2_QML_DIR}/panels/EasySetupPanel.qml

    # Debug
    ${BTC2_QML_DIR}/debug/DebugWindow.qml
    ${BTC2_QML_DIR}/debug/BoundingBox.qml
)

###############
#   Translations
###############
# set(BTC2_TRANSLATIONS_FILES
#     ${CMAKE_SOURCE_DIR}/translations/${CMAKE_PROJECT_NAME}_fr.ts
# )

# set(BTC2_TRANSLATIONS_FILES_CACHED ${BTC2_TRANSLATIONS_FILES} CACHE STRING "Translation files for the project")

############################
#
#       CMake commands
#
############################

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${BTC2_SOURCES})

qt_add_qml_module(${CMAKE_PROJECT_NAME}
    URI ${CMAKE_PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${BTC2_QML_FILES}
      RESOURCES
        ${BTC2_QML_DIR}/utils/qmldir
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  spdlog::spdlog

  libGitVersion::libGitVersion
  libLogger::libLogger
  libQSDL::libQSDL
  libWinUtils::libWinUtils
  libDataStructures::libDataStructures
  libUtils::libUtils

  Qt6::Widgets
  Qt6::Core
  Qt6::Quick
  Qt6::QuickControls2
  Qt6::Concurrent
  Qt6::Xml
)

if(CUSTOM_BUILD_TRANSLATIONS)
  qt_add_translations(
    TARGETS ${CMAKE_PROJECT_NAME} libDataStructures
    TS_FILE_DIR ${CMAKE_SOURCE_DIR}/translations
    IMMEDIATE_CALL
  )
  # set_source_files_properties(${BTC2_TRANSLATIONS_FILES}
  #     PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/translations")
  add_dependencies(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lupdate)
  add_dependencies(${CMAKE_PROJECT_NAME} update_translations)
endif()

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

)

include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
