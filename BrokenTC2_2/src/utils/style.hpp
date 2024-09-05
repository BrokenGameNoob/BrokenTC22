#pragma once

#include <QColor>
#include <QObject>
#include <QQmlEngine>

#include <Logger/logger.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

class Style : public QObject {
  Q_OBJECT
  QML_SINGLETON

  /* Theme */
  QML_LITERAL_CONSTANT(QString, kAccent, "#F7E733");
  QML_LITERAL_CONSTANT(QString, kPrimary, "#E86A92");
  QML_LITERAL_CONSTANT(QString, kForeground, "#F7F7F9");
  QML_LITERAL_CONSTANT(QString, kBackgroundColor, "#2B2D42");

  QML_LITERAL_CONSTANT(QString, kLightGrey, "#b9bac2");

  /* Graphics */
  QML_LITERAL_CONSTANT(qreal, kStandardMargin, 10.);
  QML_LITERAL_CONSTANT(qreal, kStandardIconWidth, 25.);

 public:
  ~Style() = default;
  static Style& I() {
    static auto instance{Style()};
    return instance;
  }

  static void Init() {
    SPDLOG_INFO("Init");

    CREGISTER_QML_SINGLETON_I(btc2, Style);
  }

 private:
  Style() = default;
};

}  // namespace btc2
