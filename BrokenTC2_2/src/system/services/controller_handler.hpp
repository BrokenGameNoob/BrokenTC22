#pragma once

#include <QObject>

#include <DataStructures/structures.hpp>
#include <QSDL/game_controller.hpp>

namespace btc2 {

class ControllerHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(QStringList controllerList READ GetControllerList NOTIFY controllerPluggedInOrOut);
  Q_PROPERTY(ControllerProfile* activeController READ QMLActiveController NOTIFY activeControllerChanged)

 signals:
  void controllerPluggedInOrOut();
  void activeControllerChanged();

 public:
  /*!
   * @brief MainRunner constructor should be ran after all the QML initialization.
   * Allowing for notifition publication
   */
  explicit ControllerHandler();

  static void Init();

  /* Global controllers info */
  QStringList GetControllerList() const;

  /* Get active controller */
  ControllerProfile* QMLActiveController() const;
  Q_INVOKABLE void SetActiveController(const QString& controller_name);

 public slots:
  void OnControllerPluggedIn(int controller_id);
  void OnControllerUnplugged(int controller_id);

  void OnButtonDown(int button);
  void OnButtonUp(int button);

 private:
  std::unique_ptr<qsdl::GameController> m_game_controller{nullptr};

  std::shared_ptr<ControllerProfile> m_active_profile{nullptr};
  std::shared_ptr<ControllerProfile> m_old_profile{nullptr}; /* Keep it alive for QML */
};

}  // namespace btc2
