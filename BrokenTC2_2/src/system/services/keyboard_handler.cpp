#include "keyboard_handler.hpp"

#include <DataStructures/path_utils.hpp>
#include <system/services/model_registry.hpp>
#include <system/services/service_manager.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

void KeyboardHandler::Init() {
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, KeyboardHandler, "CppOwned");
}

KeyboardHandler::KeyboardHandler() : m_active_keyboard_profile{ModelRegistry::GetKeyboardProfile()} {
  connect(win::WindowsEventSingleton::I(), &win::WindowsEventSingleton::keyDown, this, &KeyboardHandler::OnKeyDown);
  connect(win::WindowsEventSingleton::I(), &win::WindowsEventSingleton::keyUp, this, &KeyboardHandler::OnKeyUp);
}

KeyboardProfile* KeyboardHandler::QMLActiveKeyboardProfile() const {
  return m_active_keyboard_profile.get();
}

void KeyboardHandler::EnterKeybindMode() {
  m_is_in_enter_keybind_mode = true;
}
void KeyboardHandler::LeaveKeybindMode() {
  m_is_in_enter_keybind_mode = false;
}
bool KeyboardHandler::GetIsInEnterKeybindMode() const {
  return m_is_in_enter_keybind_mode;
}

QString KeyboardHandler::GetKeyName(int key) {
  return win::VkCodeToStr(key);
}

void KeyboardHandler::OnKeyDown(int key) {
  emit keyDown(key);

  if (m_is_in_enter_keybind_mode) {
    return;
  }

  /* -- actions here -- */

  if (key == m_active_keyboard_profile->SwitchGearMode()) {
    ServiceManager::GetGearHandler().CycleMode();
  } else if (key == m_active_keyboard_profile->EnableDisableInputs()) {
    ServiceManager::GetGearHandler().SetUserEnabled(!ServiceManager::GetGearHandler().IsUserEnabled());
  } else if (key == m_active_keyboard_profile->GearUp()) {
    ServiceManager::GetGearHandler().GearUp();
  } else if (key == m_active_keyboard_profile->GearDown()) {
    ServiceManager::GetGearHandler().GearDown();
  }
}

void KeyboardHandler::OnKeyUp(int key) {
  emit keyUp(key);

  if (m_is_in_enter_keybind_mode) {
    return;
  }

  /* -- actions here -- */
}

}  // namespace btc2
