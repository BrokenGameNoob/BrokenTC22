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
  FUNC(bool, OverlayEnabled, "overlay", DataEditor::SWITCH, QObject::tr("Enable overlay"), Game::kAll, true)

DS_DECLARE_STRUCT(ApplicationSettings, ApplicationSettings_STRUCT_ELEMENTS_LIST);

/* -- Soft controls tabs -- */

#define ControllerProfile_STRUCT_ELEMENTS_LIST(FUNC)                                                       \
  FUNC(QString, Name, "global", DataEditor::RAW_DISPLAY, QObject::tr("Profile name"), Game::kAll, true)    \
  FUNC(double, DeadZone, "global", DataEditor::SLIDER, QObject::tr("Axis deadzone"), Game::kAll, true)     \
  FUNC(int32_t, Clutch, "global", DataEditor::CONTROLLER_KEY, QObject::tr("Clutch key"), Game::kAll, true) \
  FUNC(int32_t, GearUp, "gear", DataEditor::RAW_DISPLAY, QObject::tr("Gear down"), Game::kAll, true)       \
  FUNC(int32_t, GearDown, "gear", DataEditor::RAW_DISPLAY, QObject::tr("Gear up"), Game::kAll, true)

DS_DECLARE_STRUCT(ControllerProfile, ControllerProfile_STRUCT_ELEMENTS_LIST);

}  // namespace btc2
