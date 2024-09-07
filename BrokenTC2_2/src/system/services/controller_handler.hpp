#pragma once

#include <QObject>

#include <QSDL/game_controller.hpp>

namespace btc2 {

class ControllerHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(QStringList controllerList READ GetControllerList NOTIFY controllerPluggedInOrOut);

 signals:
  void controllerPluggedInOrOut();

 public:
  /*!
   * @brief MainRunner constructor should be ran after all the QML initialization.
   * Allowing for notifition publication
   */
  explicit ControllerHandler();

  static void Init();

  /* Global controllers info */
  QStringList GetControllerList() const;

 public slots:
  void OnControllerPluggedIn(int controller_id);
  void OnControllerUnplugged(int controller_id);

  void OnButtonDown(int button);
  void OnButtonUp(int button);

 private:
  std::unique_ptr<qsdl::GameController> m_game_controller{nullptr};
};

}  // namespace btc2
