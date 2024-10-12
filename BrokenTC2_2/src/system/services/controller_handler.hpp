#pragma once

#include <QObject>

#include <DataStructures/structures.hpp>
#include <QSDL/game_controller.hpp>

namespace btc2 {

class ControllerHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(QStringList controllerList READ GetControllerList NOTIFY controllerPluggedInOrOut);
  Q_PROPERTY(ControllerProfile* activeController READ QMLActiveController NOTIFY activeControllerChanged)
  Q_PROPERTY(QList<ControllerProfile*> knownControllersProfiles READ QMLGetKnownProfiles NOTIFY
                 knownControllersProfilesUpdated)

  Q_PROPERTY(bool isInEnterKeybindMode READ GetIsInEnterKeybindMode NOTIFY enterKeybindModeChanged)

 signals:
  void controllerPluggedInOrOut();
  void activeControllerChanged();
  void knownControllersProfilesUpdated();

  void enterKeybindModeChanged();

  void buttonDown(int button);
  void buttonUp(int button);

 public:
  static constexpr auto kDefaultControllerProfileName{"-"};
  /*!
   * @brief MainRunner constructor should be ran after all the QML initialization.
   * Allowing for notifition publication
   */
  explicit ControllerHandler();

  static void Init();

  /* Global controllers info */
  QStringList GetControllerList() const;
  QList<ControllerProfile*> QMLGetKnownProfiles() const;

  /* Get active controller */
  ControllerProfile* QMLActiveController() const;
  Q_INVOKABLE void SetActiveController(const QString& controller_name);

  bool GetIsInEnterKeybindMode() const;

 public slots:
  Q_INVOKABLE void RefreshKnownControllersFromDisk();
  Q_INVOKABLE void SortKnownControllers();
  Q_INVOKABLE void ReorderFromList(QList<ControllerProfile*> new_profile_list);

  void OnControllerPluggedIn(int controller_id);
  void OnControllerUnplugged(int controller_id);

  void OnButtonDown(int button);
  void OnButtonUp(int button);

  Q_INVOKABLE void EnterKeybindMode();
  Q_INVOKABLE void LeaveKeybindMode();

 private:
  int ControllerIdForName(const QString& controller_name) const;
  void SetActiveController(int controller_id);
  void SetActiveController(std::shared_ptr<ControllerProfile> controller_profile);
  void AutoSetActivePrefferedController();

 private:
  std::unique_ptr<qsdl::GameController> m_game_controller{nullptr};

  std::shared_ptr<ControllerProfile> m_active_profile{nullptr};
  std::shared_ptr<ControllerProfile> m_old_profile{nullptr}; /* Keep it alive for QML */

  std::vector<std::shared_ptr<ControllerProfile>> m_known_controller_profiles{};

  bool m_is_in_enter_keybind_mode{false};
};

}  // namespace btc2
