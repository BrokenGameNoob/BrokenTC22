#include "keyboard_handler.hpp"

#include <DataStructures/path_utils.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

void KeyboardHandler::Init() {
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, KeyboardHandler, "CppOwned");
}

KeyboardHandler::KeyboardHandler()
    : m_active_keyboard_profile{std::make_unique<KeyboardProfile>(path::GetKeyboardProfilePath(), nullptr)} {
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
}

void KeyboardHandler::OnKeyUp(int key) {
  emit keyUp(key);

  if (m_is_in_enter_keybind_mode) {
    return;
  }

  /* -- actions here -- */
}

}  // namespace btc2
