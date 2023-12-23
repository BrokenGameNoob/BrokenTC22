#pragma once

#include <cassert>
#include <soft_controls.qpb.h>

#include <Logger/logger.hpp>

#include "proto_helper.hpp"

namespace {
template <typename T>
constexpr void assert_eq(T val, T val2, const auto& error_text) {
  if (val != val2) {
    SPDLOG_CRITICAL("Failed eq assert: {}\n{} != {}", error_text, val, val2);
    throw std::runtime_error{"Failed assert"};
  }
}
}  // namespace

namespace btc2 {

#define DECLARE_ACTION_COMPATIBILITY(action, kb_compatible, controller_compatible)

// Helper macro to define action compatibilities
/* F(action, kb_compatible, controller_compatible) */
#define SET_ACTION_COMPATIBILITIES(F)                           \
  F(SoftAction::Available::NONE, false, false)       \
  F(SoftAction::Available::GEAR_UP, false, true)     \
  F(SoftAction::Available::GEAR_DOWN, false, true)   \
  F(SoftAction::Available::REVERSE, false, true)     \
  F(SoftAction::Available::GEAR_1, true, true)       \
  F(SoftAction::Available::GEAR_2, false, true)      \
  F(SoftAction::Available::GEAR_3, false, true)      \
  F(SoftAction::Available::GEAR_4, false, true)      \
  F(SoftAction::Available::GEAR_5, false, true)      \
  F(SoftAction::Available::GEAR_6, false, true)      \
  F(SoftAction::Available::GEAR_7, false, true)      \
  F(SoftAction::Available::SWITCH_MODE, false, true) \
  F(SoftAction::Available::CYCLE_CONTROLLER, false, true)

// Helper macro to declare action compatibility
#define DECLARE_KEYBOARD_COMPATIBILITY(action, kb_compatible, controller_compatible) {action, kb_compatible},
#define DECLARE_CONTROLLER_COMPATIBILITY(action, kb_compatible, controller_compatible) {action, controller_compatible},
#define DECLARE_COMPATIBILITY_MAP(map_name, ...)                                       \
  inline const auto& Get##map_name##Compatibility() {                                  \
    static const std::map<SoftAction::Available, bool> kMap{{__VA_ARGS__}}; \
    return kMap;                                                                       \
  }

// Example usage of the macros
DECLARE_COMPATIBILITY_MAP(Keyboard, SET_ACTION_COMPATIBILITIES(DECLARE_KEYBOARD_COMPATIBILITY))
DECLARE_COMPATIBILITY_MAP(Controller, SET_ACTION_COMPATIBILITIES(DECLARE_CONTROLLER_COMPATIBILITY))

inline void RunSoftControlsCompatibilityCountCheck() {
  ::assert_eq(static_cast<int32_t>(btc2::GetKeyboardCompatibility().size()),
              btc2::EnumValuesCount<btc2::SoftAction::Available>(),
              "Keyboard actions compatibility maps doesn't have the same count of value as in the actions enum");
  ::assert_eq(static_cast<int32_t>(btc2::GetControllerCompatibility().size()),
              btc2::EnumValuesCount<btc2::SoftAction::Available>(),
              "Controller actions compatibility maps doesn't have the same count of value as in the actions enum");
}

inline bool IsKeyboardCompatible(const SoftAction::Available& action) {
  const auto& it{GetKeyboardCompatibility().find(action)};
  return it->second;
}

inline bool IsControllerCompatible(const SoftAction::Available& action) {
  const auto& it{GetControllerCompatibility().find(action)};
  return it->second;
}

}  // namespace btc2
