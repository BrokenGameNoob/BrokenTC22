#pragma once

#include <QColor>
#include <QObject>
#include <QQmlEngine>

#include <Logger/logger.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

class Constants : public QObject {
  Q_OBJECT
  QML_SINGLETON

  /* Icons */
  QML_LITERAL_CONSTANT(QString, kIconController, "qrc:/icons/icon_controller.svg");
  QML_LITERAL_CONSTANT(QString, kIconKeyboard, "qrc:/icons/icon_keyboard.svg");
  QML_LITERAL_CONSTANT(QString, kIconSettings, "qrc:/icons/icon_settings.svg");

 public:
  ~Constants() = default;
  static Constants& I() {
    static auto instance{Constants()};
    return instance;
  }

  static void Init() {
    SPDLOG_INFO("Init");

    CREGISTER_QML_SINGLETON_I(btc2, Constants);
  }

 private:
  Constants() = default;
};

}  // namespace btc2
