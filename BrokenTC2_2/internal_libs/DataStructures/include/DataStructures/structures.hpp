#pragma once

#include <DataStructures/structures_macros.hpp>

namespace btc2 {

class DummyForceAutoMocProcess0 : public QObject {
  Q_OBJECT
};

/* ELEMENTS_LIST(type, Name, editor_group_name, DataEditor::EditorType, editor_title, game_compatibility,
 * display_condition) */
// TODO: Add the display_condition in the code. It's not implemented yet.

#define ApplicationSettings_STRUCT_ELEMENTS_LIST(FUNC) \
  FUNC(bool,                                           \
       OverlayEnabled,                                 \
       DS_DEFAULT,                                     \
       "overlay",                                      \
       DataEditor::SWITCH,                             \
       QObject::tr("Enable overlay"),                  \
       Game::kAll,                                     \
       true)                                           \
  FUNC(int32_t, OpenedTab, DS_DEFAULT, "", DataEditor::NO_EDITOR, "", Game::kNone, false)

DS_DECLARE_STRUCT(ApplicationSettings, ApplicationSettings_STRUCT_ELEMENTS_LIST);

/* -- Soft controls tabs -- */

#define KeyboardProfile_STRUCT_ELEMENTS_LIST(FUNC) \
  FUNC(int32_t,                                    \
       SwitchGearMode,                             \
       DS_DEFAULT,                                 \
       "soft_controls",                            \
       DataEditor::KEYBOARD_KEY,                   \
       QObject::tr("Switch gear mode"),            \
       Game::kAll,                                 \
       true)

DS_DECLARE_STRUCT(KeyboardProfile, KeyboardProfile_STRUCT_ELEMENTS_LIST);

// TODO: Add default value
#define ControllerProfile_STRUCT_ELEMENTS_LIST(FUNC)                                                               \
  FUNC(int32_t, PriorityIndex, -1, "global", DataEditor::RAW_DISPLAY, "DEBUG prio index", Game::kAll, true)        \
  FUNC(QString, Name, "Unknown", "global", DataEditor::RAW_DISPLAY, QObject::tr("Profile name"), Game::kAll, true) \
  FUNC(double, DeadZone, 0.5, "global", DataEditor::SLIDER, QObject::tr("Axis deadzone"), Game::kAll, true)        \
  FUNC(int32_t,                                                                                                    \
       Clutch,                                                                                                     \
       def::kControllerButton,                                                                                     \
       "global",                                                                                                   \
       DataEditor::CONTROLLER_KEY,                                                                                 \
       QObject::tr("Clutch key"),                                                                                  \
       Game::kAll,                                                                                                 \
       true)                                                                                                       \
  FUNC(bool,                                                                                                       \
       SkipNeutral,                                                                                                \
       def::kControllerButton,                                                                                     \
       "global",                                                                                                   \
       DataEditor::SWITCH,                                                                                         \
       QObject::tr("Skip neutral"),                                                                                \
       Game::kAll,                                                                                                 \
       true)                                                                                                       \
  FUNC(int32_t,                                                                                                    \
       GearUp,                                                                                                     \
       def::kControllerButton,                                                                                     \
       "gear",                                                                                                     \
       DataEditor::RAW_DISPLAY,                                                                                    \
       QObject::tr("Gear down"),                                                                                   \
       Game::kAll,                                                                                                 \
       true)                                                                                                       \
  FUNC(int32_t,                                                                                                    \
       GearDown,                                                                                                   \
       def::kControllerButton,                                                                                     \
       "gear",                                                                                                     \
       DataEditor::RAW_DISPLAY,                                                                                    \
       QObject::tr("Gear up"),                                                                                     \
       Game::kAll,                                                                                                 \
       true)

DS_DECLARE_STRUCT(ControllerProfile, ControllerProfile_STRUCT_ELEMENTS_LIST);

}  // namespace btc2
