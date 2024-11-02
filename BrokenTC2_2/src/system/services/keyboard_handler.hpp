#pragma once

#include <QObject>

#include <DataStructures/structures.hpp>
#include <WinUtils/event_handler.hpp>

namespace btc2 {

class KeyboardHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(KeyboardProfile* profile READ QMLActiveKeyboardProfile NOTIFY activeKeyboardProfileChanged)

  Q_PROPERTY(bool isInEnterKeybindMode READ GetIsInEnterKeybindMode NOTIFY enterKeybindModeChanged)

 signals:
  void activeKeyboardProfileChanged();

  void enterKeybindModeChanged();

  void keyDown(int key);
  void keyUp(int key);

 public:
  static void Init();

 public:
  explicit KeyboardHandler();

  KeyboardProfile* QMLActiveKeyboardProfile() const;

  void OnKeyDown(int key);
  void OnKeyUp(int key);

  Q_INVOKABLE void EnterKeybindMode();
  Q_INVOKABLE void LeaveKeybindMode();
  bool GetIsInEnterKeybindMode() const;

  Q_INVOKABLE static QString GetKeyName(int key);

 private:
  std::shared_ptr<KeyboardProfile> m_active_keyboard_profile{nullptr};

  bool m_is_in_enter_keybind_mode{false};
};

}  // namespace btc2
