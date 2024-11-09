#pragma once

#include <DataStructures/structures_macros.hpp>

namespace btc2 {

class DummyForceAutoMocProcess0 : public QObject {
  Q_OBJECT
};

/* ELEMENTS_LIST(type, Name, editor_group_name, DataEditor::EditorType, editor_title, game_compatibility,
 * display_condition) */
// TODO: Add the display_condition in the code. It's not implemented yet.

#define ApplicationSettings_STRUCT_ELEMENTS_LIST(FUNC)                                                            \
  FUNC(bool, OverlayEnabled, true, "overlay", DataEditor::SWITCH, QObject::tr("Enable overlay"), Game::ALL, true) \
  FUNC(bool,                                                                                                      \
       OverlayAlwaysVisible,                                                                                      \
       false,                                                                                                     \
       "overlay",                                                                                                 \
       DataEditor::SWITCH,                                                                                        \
       QObject::tr("Make overlay always visible"),                                                                \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t, OpenedTab, DS_DEFAULT, "", DataEditor::NO_EDITOR, "", Game::NONE, false)                          \
  FUNC(QString, SelectedGameName, DS_DEFAULT, "", DataEditor::NO_EDITOR, "", Game::NONE, false)                   \
  FUNC(QString,                                                                                                   \
       SelectedOverlayScreen,                                                                                     \
       DS_DEFAULT,                                                                                                \
       "overlay",                                                                                                 \
       DataEditor::SCREEN_SELECTOR,                                                                               \
       QObject::tr("Overlay screen"),                                                                             \
       Game::ALL,                                                                                                 \
       false)                                                                                                     \
  FUNC(QString,                                                                                                   \
       BackgroundImage,                                                                                           \
       DS_DEFAULT,                                                                                                \
       "soft",                                                                                                    \
       DataEditor::FILE_SELECTOR,                                                                                 \
       QObject::tr("Background image"),                                                                           \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(double,                                                                                                    \
       BackgroundImageOpacity,                                                                                    \
       1.,                                                                                                        \
       "soft",                                                                                                    \
       DataEditor::SLIDER,                                                                                        \
       QObject::tr("Background image opacity"),                                                                   \
       Game::ALL,                                                                                                 \
       true)

DS_DECLARE_STRUCT(ApplicationSettings, ApplicationSettings_STRUCT_ELEMENTS_LIST);

/* -- Soft controls tabs -- */

#define KeyboardProfile_STRUCT_ELEMENTS_LIST(FUNC) \
  FUNC(int32_t,                                    \
       SwitchGearMode,                             \
       def::kDefaultKey,                           \
       "soft_controls",                            \
       DataEditor::KEYBOARD_KEY,                   \
       QObject::tr("Switch gear mode"),            \
       Game::ALL,                                  \
       true)                                       \
  FUNC(int32_t,                                    \
       EnableDisableInputs,                        \
       def::kDefaultKey,                           \
       "soft_controls",                            \
       DataEditor::KEYBOARD_KEY,                   \
       QObject::tr("Enable / disable"),            \
       Game::ALL,                                  \
       true)                                       \
  FUNC(int32_t,                                    \
       GearUp,                                     \
       def::kDefaultKey,                           \
       "sequential_gear",                          \
       DataEditor::KEYBOARD_KEY,                   \
       QObject::tr("Gear up"),                     \
       Game::ALL,                                  \
       true)                                       \
  FUNC(int32_t,                                    \
       GearDown,                                   \
       def::kDefaultKey,                           \
       "sequential_gear",                          \
       DataEditor::KEYBOARD_KEY,                   \
       QObject::tr("Gear down"),                   \
       Game::ALL,                                  \
       true)

DS_DECLARE_STRUCT(KeyboardProfile, KeyboardProfile_STRUCT_ELEMENTS_LIST);

// TODO: Add default value
#define ControllerProfile_STRUCT_ELEMENTS_LIST(FUNC)                                                              \
  FUNC(int32_t, PriorityIndex, -1, "global", DataEditor::NO_EDITOR, "", Game::ALL, true)                          \
  FUNC(QString, Name, "Unknown", "global", DataEditor::RAW_DISPLAY, QObject::tr("Profile name"), Game::ALL, true) \
  FUNC(double, DeadZone, 0.5, "global", DataEditor::SLIDER, QObject::tr("Axis deadzone"), Game::ALL, true)        \
  FUNC(int32_t,                                                                                                   \
       GearUp,                                                                                                    \
       def::kControllerButton,                                                                                    \
       "sequential_gear",                                                                                         \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear up"),                                                                                    \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       GearDown,                                                                                                  \
       def::kControllerButton,                                                                                    \
       "sequential_gear",                                                                                         \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear down"),                                                                                  \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       GearReverse,                                                                                               \
       def::kControllerButton,                                                                                    \
       "gear",                                                                                                    \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear R"),                                                                                     \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       Gear1,                                                                                                     \
       def::kControllerButton,                                                                                    \
       "gear",                                                                                                    \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear 1"),                                                                                     \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       Gear2,                                                                                                     \
       def::kControllerButton,                                                                                    \
       "gear",                                                                                                    \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear 2"),                                                                                     \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       Gear3,                                                                                                     \
       def::kControllerButton,                                                                                    \
       "gear",                                                                                                    \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear 3"),                                                                                     \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       Gear4,                                                                                                     \
       def::kControllerButton,                                                                                    \
       "gear",                                                                                                    \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear 4"),                                                                                     \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       Gear5,                                                                                                     \
       def::kControllerButton,                                                                                    \
       "gear",                                                                                                    \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear 5"),                                                                                     \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       Gear6,                                                                                                     \
       def::kControllerButton,                                                                                    \
       "gear",                                                                                                    \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear 6"),                                                                                     \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       Gear7,                                                                                                     \
       def::kControllerButton,                                                                                    \
       "gear",                                                                                                    \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Gear 7"),                                                                                     \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       SwitchGearMode,                                                                                            \
       def::kControllerButton,                                                                                    \
       "soft_controls",                                                                                           \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Switch gear mode"),                                                                           \
       Game::ALL,                                                                                                 \
       true)                                                                                                      \
  FUNC(int32_t,                                                                                                   \
       EnableDisableInputs,                                                                                       \
       def::kControllerButton,                                                                                    \
       "soft_controls",                                                                                           \
       DataEditor::CONTROLLER_KEY,                                                                                \
       QObject::tr("Enable / disable"),                                                                           \
       Game::ALL,                                                                                                 \
       true)

DS_DECLARE_STRUCT(ControllerProfile, ControllerProfile_STRUCT_ELEMENTS_LIST);

/* -- Game config tabs -- */

#define GameProfileTheCrew_STRUCT_ELEMENTS_LIST(FUNC) \
  FUNC(int32_t,                                       \
       GearUp,                                        \
       def::kControllerButton,                        \
       "gear_seq",                                    \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear up"),                        \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       GearDown,                                      \
       def::kControllerButton,                        \
       "gear_seq",                                    \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear down"),                      \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       Clutch,                                        \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Clutch"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       GearReverse,                                   \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear R"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       Gear1,                                         \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear 1"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       Gear2,                                         \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear 2"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       Gear3,                                         \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear 3"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       Gear4,                                         \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear 4"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       Gear5,                                         \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear 5"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       Gear6,                                         \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear 6"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(int32_t,                                       \
       Gear7,                                         \
       def::kControllerButton,                        \
       "gear",                                        \
       DataEditor::KEYBOARD_KEY,                      \
       QObject::tr("Gear 7"),                         \
       Game::ALL,                                     \
       true)                                          \
  FUNC(bool,                                          \
       UseSequentialAfterClutch,                      \
       true,                                          \
       "behavior",                                    \
       DataEditor::SWITCH,                            \
       QObject::tr("Use sequential after clutch"),    \
       Game::ALL,                                     \
       true)

DS_DECLARE_STRUCT(GameProfileTheCrew, GameProfileTheCrew_STRUCT_ELEMENTS_LIST);

constexpr auto kDefaultOverlayBackgroundColor{"#50000000"};

#define GameOverlay_STRUCT_ELEMENTS_LIST(FUNC)                                                                         \
  FUNC(double, SoftRunningIndicatorX, DS_DEFAULT, "soft_running", DataEditor::NO_EDITOR, "", Game::NONE, false)        \
  FUNC(double, SoftRunningIndicatorY, DS_DEFAULT, "soft_running", DataEditor::NO_EDITOR, "", Game::NONE, false)        \
  FUNC(double, SoftRunningIndicatorScaling, 1., "soft_running", DataEditor::NO_EDITOR, "", Game::NONE, false)          \
  FUNC(bool,                                                                                                           \
       SoftRunningIndicatorEnabled,                                                                                    \
       true,                                                                                                           \
       "soft_running",                                                                                                 \
       DataEditor::SWITCH,                                                                                             \
       QObject::tr("Enable"),                                                                                          \
       Game::ALL,                                                                                                      \
       true)                                                                                                           \
  FUNC(double, GearX, DS_DEFAULT, "gear", DataEditor::NO_EDITOR, "", Game::NONE, false)                                \
  FUNC(double, GearY, DS_DEFAULT, "gear", DataEditor::NO_EDITOR, "", Game::NONE, false)                                \
  FUNC(double, GearScaling, 1., "gear", DataEditor::NO_EDITOR, "", Game::NONE, false)                                  \
  FUNC(bool, GearEnabled, true, "gear", DataEditor::SWITCH, QObject::tr("Enable"), Game::ALL, true)                    \
  FUNC(bool,                                                                                                           \
       GearChangeGearColorDependingOnMode,                                                                             \
       true,                                                                                                           \
       "gear",                                                                                                         \
       DataEditor::SWITCH,                                                                                             \
       QObject::tr("Change color"),                                                                                    \
       Game::ALL,                                                                                                      \
       true)                                                                                                           \
  FUNC(bool,                                                                                                           \
       GearIndicatorModeNotifEnabled,                                                                                  \
       false,                                                                                                          \
       "gear",                                                                                                         \
       DataEditor::SWITCH,                                                                                             \
       QObject::tr("Mode notifs"),                                                                                     \
       Game::ALL,                                                                                                      \
       true)                                                                                                           \
  FUNC(double, NotifX, DS_DEFAULT, "notif", DataEditor::NO_EDITOR, "", Game::NONE, false)                              \
  FUNC(double, NotifY, DS_DEFAULT, "notif", DataEditor::NO_EDITOR, "", Game::NONE, false)                              \
  FUNC(double, NotifScaling, 1., "notif", DataEditor::NO_EDITOR, "", Game::NONE, false)                                \
  FUNC(bool, NotifEnabled, true, "notif", DataEditor::SWITCH, QObject::tr("Enable"), Game::ALL, true)                  \
  FUNC(QString,                                                                                                        \
       NotifBackgroundColor,                                                                                           \
       kDefaultOverlayBackgroundColor,                                                                                 \
       "notif",                                                                                                        \
       DataEditor::COLOR,                                                                                              \
       QObject::tr("Background"),                                                                                      \
       Game::ALL,                                                                                                      \
       true)                                                                                                           \
  FUNC(double, DebugLabelX, DS_DEFAULT, "debug", DataEditor::NO_EDITOR, "", Game::NONE, false)                         \
  FUNC(double, DebugLabelY, DS_DEFAULT, "debug", DataEditor::NO_EDITOR, "", Game::NONE, false)                         \
  FUNC(double, DebugLabelScaling, 1., "debug", DataEditor::NO_EDITOR, "", Game::NONE, false)                           \
  FUNC(bool, DebugLabelEnabled, false, "debug", DataEditor::SWITCH, QObject::tr("Enable"), Game::ALL, true)            \
  FUNC(double, ModeIndicatorX, DS_DEFAULT, "mode", DataEditor::NO_EDITOR, "", Game::NONE, false)                       \
  FUNC(double, ModeIndicatorY, DS_DEFAULT, "mode", DataEditor::NO_EDITOR, "", Game::NONE, false)                       \
  FUNC(double, ModeIndicatorScaling, 1., "mode", DataEditor::NO_EDITOR, "", Game::NONE, false)                         \
  FUNC(bool, ModeIndicatorEnabled, true, "mode", DataEditor::SWITCH, QObject::tr("Enable"), Game::ALL, true)           \
  FUNC(bool,                                                                                                           \
       ModeIndicatorChangeGearColorDependingOnMode,                                                                    \
       true,                                                                                                           \
       "mode",                                                                                                         \
       DataEditor::SWITCH,                                                                                             \
       QObject::tr("Change color"),                                                                                    \
       Game::ALL,                                                                                                      \
       true)                                                                                                           \
  FUNC(                                                                                                                \
      QString, ClutchColor, "#FFFFFFFF", "mode,gear", DataEditor::COLOR, QObject::tr("Clutch color"), Game::ALL, true) \
  FUNC(QString,                                                                                                        \
       NoClutchColor,                                                                                                  \
       "#FFFF0000",                                                                                                    \
       "mode,gear",                                                                                                    \
       DataEditor::COLOR,                                                                                              \
       QObject::tr("No clutch color"),                                                                                 \
       Game::ALL,                                                                                                      \
       true)                                                                                                           \
  FUNC(QString,                                                                                                        \
       DisabledColor,                                                                                                  \
       "#8f8f8f",                                                                                                      \
       "mode,gear",                                                                                                    \
       DataEditor::COLOR,                                                                                              \
       QObject::tr("Disabled color"),                                                                                  \
       Game::ALL,                                                                                                      \
       true)                                                                                                           \
  FUNC(QString,                                                                                                        \
       GearBackgroundColor,                                                                                            \
       kDefaultOverlayBackgroundColor,                                                                                 \
       "mode,gear",                                                                                                    \
       DataEditor::COLOR,                                                                                              \
       QObject::tr("Background"),                                                                                      \
       Game::ALL,                                                                                                      \
       true)

DS_DECLARE_STRUCT(GameOverlayData, GameOverlay_STRUCT_ELEMENTS_LIST);

}  // namespace btc2
